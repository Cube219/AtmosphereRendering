#version 430 core

// vertex attributes
layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texcoord;

layout (location=0) out vec3 Normal;

uniform mat4 MVP;

void main()
{
	Normal = normal;
	gl_Position = MVP * vec4(position, 1);
}
