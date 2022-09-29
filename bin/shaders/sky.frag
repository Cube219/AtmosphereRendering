#version 430 core

layout (location=0) in vec3 Normal;
layout (location=0) out vec4 color;

uniform vec3 cameraPos;

uniform vec3 lightDir;
uniform float g;
uniform vec3 invWaveLength4;

uniform float innerRadius;
uniform float outerRadius;
uniform float Kr;
uniform float Km;
uniform float sun;
uniform float Scale;

float rayleighPhase(float cos)
{
	return 0.75 + 0.75 * cos * cos;
}

float miePhase(float cos, float g)
{
	float g2 = g * g;
	float cos2 = cos * cos;
	return 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + cos2) / pow(1.0 + g2 - 2.0 * g * cos, 1.5);
}

float scale(float cos)
{
	float x = 1.0 - cos;
	return 0.25 * exp(-0.00287 + x * (0.459 + x * (3.83 + x * (-6.80 + x * 5.25))));
}

void main()
{
	float PI4 = 4.0 * 3.141593;

	vec3 outerPos = Normal * outerRadius;

	vec3 toOuterFromCam = outerPos - cameraPos;
	float rayLength = length(toOuterFromCam);
	toOuterFromCam /= rayLength;

	vec3 startPos = cameraPos;

	float startHeight = exp(0);
	float startRayAngle = dot(toOuterFromCam, startPos) / length(startPos);
	float startDepth = startHeight * scale(startRayAngle);

	const int sampleNum = 8;
	float stepLength = rayLength / float(sampleNum);
	vec3 rayStep = toOuterFromCam * stepLength;
	vec3 curPos = startPos + rayStep * 0.5;

	vec3 calculatedColor = vec3(0.0);
	for (int i = 0; i < sampleNum; ++i)
	{
		float height = exp(0);
		float lightAngle = dot(lightDir, curPos) / length(curPos);
		float cameraAngle = dot(toOuterFromCam, curPos) / length(curPos);
		float scatter = startDepth - height * scale(cameraAngle) + height * scale(lightAngle);
		vec3 attenuate = exp(-scatter * (invWaveLength4 * Kr * PI4 + Km * PI4));

		calculatedColor += attenuate * (height * stepLength * Scale);
		curPos += rayStep;
	}

	vec3 rayleighColor = calculatedColor * (invWaveLength4 * Kr * sun);
	vec3 mieColor = calculatedColor * Km * sun;

	float cos = dot(lightDir, -toOuterFromCam) / length(toOuterFromCam);
	color = vec4(rayleighPhase(cos) * rayleighColor + miePhase(cos, g) * mieColor, 1.0);
	color.a = 1.0;

	color = 1.0 - exp(color * -2.0);
}
