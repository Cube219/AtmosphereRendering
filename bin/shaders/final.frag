#version 330

in vec2 tc;

out vec4 fragColor;

uniform sampler2D colorMap;

void main()
{
	fragColor = texture(colorMap, tc);
}
