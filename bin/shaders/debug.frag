#version 430 core

uniform sampler3D debugTex;

uniform int channel;

in vec2 tc;

out vec4 fragColor;

void main()
{
	vec4 color = vec4(0, 0, 0, 1);
	
	color = texture(debugTex, vec3(tc, 0));
	
	if(channel == 0) {
		fragColor = vec4(color.rrr, 1);
	} else if(channel == 1) {
		fragColor = vec4(color.ggg, 1);
	} else if(channel == 2) {
		fragColor = vec4(color.bbb, 1);
	} else if(channel == 3) {
		fragColor = vec4(color.aaa, 1);
	}
}
