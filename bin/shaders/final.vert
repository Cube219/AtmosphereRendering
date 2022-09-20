#version 330

// vertex attributes
layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texcoord;

out vec2 tc;

void main()
{
	gl_Position = vec4(position, 1);
	tc = texcoord;
}
