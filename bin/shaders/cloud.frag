#version 430 core

layout (location=0) out vec4 color;

layout (location=0) in vec2 tc;

uniform sampler2D depthMap;

// Camera info
uniform vec2 screenSize;
uniform float FOV;
uniform vec3 camPos;
uniform mat4 projView;
uniform mat4 invView;

// Sphere info
uniform vec3 sphereCenter;
uniform float innerSphereRadius;
uniform float outerSphereRadius;

// Light info
uniform vec3 lightDir;

// Noise textures
uniform sampler3D shapeTex;
uniform sampler3D detailTex;
// weather texture
uniform sampler2D weatherMap;


// CONSTANTS
// Random offset added to starting ray depth to prevent banding artifacts 
#define BAYER_FACTOR 1.0/16.0
uniform float bayerFilter[16u] = float[]
(
	0.0*BAYER_FACTOR, 8.0*BAYER_FACTOR, 2.0*BAYER_FACTOR, 10.0*BAYER_FACTOR,
	12.0*BAYER_FACTOR, 4.0*BAYER_FACTOR, 14.0*BAYER_FACTOR, 6.0*BAYER_FACTOR,
	3.0*BAYER_FACTOR, 11.0*BAYER_FACTOR, 1.0*BAYER_FACTOR, 9.0*BAYER_FACTOR,
	15.0*BAYER_FACTOR, 7.0*BAYER_FACTOR, 13.0*BAYER_FACTOR, 5.0*BAYER_FACTOR
);

// Cloud types height density gradients
#define STRATUS_GRADIENT vec4(0.0, 0.1, 0.2, 0.3)
#define STRATOCUMULUS_GRADIENT vec4(0.02, 0.2, 0.48, 0.625)
#define CUMULUS_GRADIENT vec4(0.00, 0.1625, 0.88, 0.98)

// Normalized height (0 - 1) within the cloud layer
float getHeightFraction(vec3 p)
{
	return (length(p - sphereCenter) - innerSphereRadius) / (outerSphereRadius - innerSphereRadius);
}

vec2 sphericalUVProj(vec3 p)
{
	vec3 dirVector = normalize(p - sphereCenter);
	return (dirVector.xz + 1.0) / 2.0;
}

// Retrieves the weather data stored in the weather texture
vec3 getWeatherData(vec3 p)
{
	vec2 uv = sphericalUVProj(p);
	return texture(weatherMap, uv).rgb;
}

// Remap utility
float remapValue(float original, float oMin, float oMax, float nMin, float nMax)
{
	return nMin + ((original - oMin) / (oMax - oMin)) * (nMax - nMin);
}

// Retrieves the cloud density based on cloud type and weighting between default cloud models
float getDensityForCloud(float heightFraction, float cloudType)
{
	float stratusFactor = 1.0 - clamp(cloudType * 2.0, 0.0, 1.0);
	float stratoCumulusFactor = 1.0 - abs(cloudType - 0.5) * 2.0;
	float cumulusFactor = clamp(cloudType - 0.5, 0.0, 1.0) * 2.0;

	vec4 baseGradient = stratusFactor * STRATUS_GRADIENT + stratoCumulusFactor * STRATOCUMULUS_GRADIENT + cumulusFactor * CUMULUS_GRADIENT;

	// gradicent computation (see Siggraph 2017 Nubis-Decima talk)
	float result = remapValue(heightFraction, baseGradient.x, baseGradient.y, 0.0, 1.0) * remapValue(heightFraction, baseGradient.z, baseGradient.w, 1.0, 0.0);
	return result;
}

float sampleCloudDensity(vec3 p, float lod, bool expensive, float heightFraction)
{
	float deltaDist = clamp(length(p - camPos), 0.0, 1.0);

	vec2 uv = sphericalUVProj(p);
	vec3 weatherData = texture(weatherMap, uv).rgb;

	// Sample base cloud shape noises (Perlin-Worley + 3 Worley)
	vec4 baseCloudNoise = textureLod(shapeTex, vec3(uv, heightFraction), lod);
	
	// Build the low frequency fbm modifier
	float lowFreqFBM = ( baseCloudNoise.g * 0.625) + ( baseCloudNoise.b * 0.25 ) + ( baseCloudNoise.a * 0.125 );
	float baseCloudShape = remapValue(baseCloudNoise.r, -(1.0 - lowFreqFBM), 1.0, 0.0, 1.0);

	// Apply density gradient based on cloud type
	float densityGradient = getDensityForCloud(heightFraction, weatherData.g);
	baseCloudShape *= densityGradient;

	// Apply coverage
	float coverage = clamp(weatherData.r, 0.0, 1.0);
	// Make sure cloud with less density than actual coverage dissapears
	float coveragedCloud = remapValue(baseCloudShape, coverage, 1.0, 0.0, 1.0);
	coveragedCloud *= coverage;
	coveragedCloud *= mix(1.0, 0.0, clamp(heightFraction / coverage, 0.0, 1.0));

	float finalCloud = coveragedCloud;

	// Only erode the cloud if erosion will be visible (low density sampled until now)
	if(expensive)
	{
		// Build−high frequency Worley noise FBM.
		vec3 erodeCloudNoise = textureLod(detailTex, vec3(uv, heightFraction), lod).rgb;
		float highFreqFBM = (erodeCloudNoise.r * 0.625) + (erodeCloudNoise.g * 0.25) + (erodeCloudNoise.b * 0.125);

		float highFreqNoiseModifier = mix(highFreqFBM, 1.0 - highFreqFBM, clamp(heightFraction * 8.5, 0.0, 1.0));
		finalCloud = remapValue(coveragedCloud, highFreqNoiseModifier * 0.8, 1.0, 0.0, 1.0);
	}

	return clamp(finalCloud, 0.0, 1.0);
}

// Scattering phase function
float henyeyGreenstein(vec3 l, vec3 v, float g, float ca)
{
	float g2 = g * g;

	return ((1.0 - g2) / pow((1.0 + g2 - 2.0 * g * ca), 1.5 )) * (1.0 / (4.0 * 3.1415));
}

// Intensity decreases with density
float beer(float density)
{
	return exp(-density);
}

float lightEnergy(vec3 l, vec3 v, float ca, float coneDensity)
{
	return 50.0 * beer(coneDensity) * henyeyGreenstein(l, v, 0.2, ca);
}

float raymarchToLight(vec3 pos, vec3 d, float stepSize)
{
	vec3 currentPos = pos;
	vec3 rayStep = lightDir * stepSize;
	// Starting cone radius. Will increase as we move along it
	float coneRadius = 1.0;
	// Sampled density until now
	float invDepth = 1.0 / (stepSize * 6);
	float density = 0.0;
	// Density - transmittance until now
	float coneDensity = 0.0;

	// Raymarch to the light
	int i = 0;
	while(i < 6) {
		// By advancing towards the light we might go outside the atmosphere
		float heightFraction = getHeightFraction(currentPos);
		if(heightFraction <= 1.0) {
			// sample the expensive way if we here at near borders (where density is low, like 0.3 or below)
			float cloudDensity = sampleCloudDensity(currentPos, float(i + 1), coneDensity < 0.3, heightFraction);
			if(cloudDensity > 0.0) {
				density += cloudDensity;
				float transmittance = 1.0 - (density * invDepth);
				coneDensity += (cloudDensity * transmittance);
			}
		}

		currentPos += rayStep;
		coneRadius += 0.15;
		i++;
	}

	float ca = dot(lightDir, d);

	// Compute light energy arriving at point
	return lightEnergy(lightDir, d, ca, coneDensity);
}

float raymarch(vec3 startPos, vec3 endPos, out vec3 color)
{
	int sampleCount = 96;
	vec3 path = endPos - startPos;

	vec3 stepVector = path / float(sampleCount - 1);
	float stepSize = length(stepVector);
	vec3 rayDir = normalize(path);

	vec3 currentPos = startPos;
	float density = 0.0;
	vec4 result = vec4(0.0);

	// Dithering on the starting ray position to reduce banding artifacts
	int a = int(gl_FragCoord.x) % 4;
	int b = int(gl_FragCoord.y) % 4;
	currentPos += stepVector * bayerFilter[a * 4 + b];

	// Light
	vec3 lc = vec3(0.5);
	vec3 lightAmbient = vec3(0.8) * 0.7;

	int i = 0;
	while(i < sampleCount) {
		float cloudDensity = sampleCloudDensity(currentPos, 0, true, getHeightFraction(currentPos));

		if(cloudDensity > 0.0) {
			density += cloudDensity;
			float lightEnergy = raymarchToLight(currentPos, rayDir, stepSize);

			vec4 src = vec4(lc * lightEnergy + lightAmbient, cloudDensity);
			src.rgb *= src.a;
			result = (1.0 - result.a) * src + result;
		}

		currentPos += stepVector;
		i++;
	}

	color = result.xyz;
	return result.a;
}

void main()
{
	vec2 fragCoord = vec2(gl_FragCoord.x, gl_FragCoord.y);

	if(texture(depthMap, vec2(fragCoord / screenSize)).x < 1.0) {
		color = vec4(0); // Occuluded
	} else {
		// Compute ray direction
		vec2 fragUV = fragCoord - screenSize / 2.0;
		float z =  screenSize.y / 2 / tan(FOV / 2);
		vec3 viewDir = normalize(vec3(fragUV, -z));
		vec3 worldDir = normalize( (invView * vec4(viewDir, 0)).xyz);
		
		vec3 startPos, endPos;
		startPos = camPos;
		endPos = startPos + worldDir * 8000000;
		
		// Raymarch
		vec3 outColor = vec3(0);
		float density = raymarch(startPos, endPos, outColor);
		density = clamp(density, 0.0, 1.0);

		color = vec4(outColor, density);
	}
}
