#version 330

// vertex attributes
layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texcoord;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

out vec2 tc;
out mat3 TBN;
out vec4 wpos;

void main()
{
	wpos = model_matrix * vec4(position,1);
	vec4 epos = view_matrix * wpos;
	gl_Position = projection_matrix * epos;

	vec3 norm, tangent, btangent;
	norm = normal;

	vec3 c1 = cross(norm, vec3(0.0, 0.0, 1.0));
	vec3 c2 = cross(norm, vec3(0.0, 1.0, 0.0));
	if(length(c1) > length(c2)) {
		tangent = c1;
	} else {
		tangent = c2;
	}
	tangent = normalize(tangent);
	btangent = normalize(cross(norm, tangent));

	norm = normalize(mat3(transpose(inverse(model_matrix)))*normal);
	tangent = normalize(mat3(model_matrix) * tangent);
	btangent = normalize(mat3(model_matrix) * btangent);
	
	TBN = mat3(tangent, btangent, norm);
	
	tc = texcoord;
}
