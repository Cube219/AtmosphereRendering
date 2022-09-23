#version 430 core

layout (location=0) out vec4 color;

in vec2 texCoord;

void main()
{
	color = vec4(0.5, 0, 0, 1);
}
