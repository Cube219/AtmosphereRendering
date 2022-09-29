#version 330

in vec2 tc;
in mat3 TBN;
in vec4 wpos;

out vec4 fragColor;

uniform vec4 debug_color;

uniform bool useShading;

uniform bool useDiffuseTexture;
uniform sampler2D diffuseTex;
uniform vec4 diffuse;

uniform bool useSpecularTexture;
uniform sampler2D specularTex;
uniform vec4 specular;

uniform bool useNormalTexture;
uniform sampler2D normalTex;

uniform vec3 cameraPos;

void main()
{
	vec3 norm;
	if(useNormalTexture == true) {
		norm = texture(normalTex, tc).rgb;
		norm = norm * 2.0 - 1.0;
		norm = normalize(TBN * norm);
	} else {
		norm = TBN[2];
	}

	vec4 diffValue = useDiffuseTexture ? texture(diffuseTex, tc) : diffuse;
	vec4 specValue = useSpecularTexture ? texture(specularTex, tc) : specular;
	
	fragColor = diffValue;
}
