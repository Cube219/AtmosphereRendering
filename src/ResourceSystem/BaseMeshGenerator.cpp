#include "BaseMeshGenerator.h"

#include "Utility.h"

SPtr<Mesh> BaseMeshGenerator::GetBoxMesh()
{
	Vector<Vertex> vertices;
	Vertex v;
	v.pos = { -0.5f, -0.5f, -0.5f };
	v.uv = { 0, 0 };
	vertices.push_back(v);
	v.pos = { -0.5f, 0.5f, -0.5f };
	v.uv = { 1, 0 };
	vertices.push_back(v);
	v.pos = { 0.5f, -0.5f, -0.5f };
	v.uv = { 0, 1 };
	vertices.push_back(v);
	v.pos = { 0.5f, 0.5f, -0.5f };
	v.uv = { 1, 1 };
	vertices.push_back(v);
	v.pos = { 0.5f, -0.5f, 0.5f };
	v.uv = { 0, 0 };
	vertices.push_back(v);
	v.pos = { 0.5f, 0.5f, 0.5f };
	v.uv = { 1, 0 };
	vertices.push_back(v);
	v.pos = { -0.5f, -0.5f, 0.5f };
	v.uv = { 0, 1 };
	vertices.push_back(v);
	v.pos = { -0.5f, 0.5f, 0.5f };
	v.uv = { 1, 1 };
	vertices.push_back(v);

	Vector<uint> indices = {
		0,1,2,  2,1,3 ,  2,3,4,  4,3,5,  4,5,6,  6,5,7,  6,7,0,  0,7,1,  6,0,2,  2,4,6,  3,1,5,  5,1,7
	};

	SetNormalVector(vertices, indices);

	SPtr<Mesh> mesh = std::make_shared<Mesh>(vertices.size(), indices.size());
	mesh->SetVertices(vertices.data(), vertices.size());
	mesh->SetIndices(indices.data(), indices.size());
	mesh->UpdateVertexArray();

	SubMesh sm;
	sm.vertexOffset = 0;
	sm.indexOffset = 0;
	sm.indexCount = indices.size();
	mesh->SetSubMesh(&sm, 1);

	return mesh;
}

SPtr<Mesh> BaseMeshGenerator::GetSphereMesh(int div)
{
	Vector<Vertex> vertices;

	float delta = 2.0f * PI / float(div);
	float t = 0.0f;
	float p = 0.0f;
	for(int i = 0; i <= div / 2; i++) {
		float sint = sin(t);
		float cost = cos(t);
		p = 0.0f;
		for(int j = 0; j <= div; j++) {
			float sinp = sin(p);
			float cosp = cos(p);

			float x = sint * cosp;
			float y = sint * sinp;
			float z = cost;

			Vertex v;
			v.pos = vec3(x, y, z);
			v.norm = vec3(x, y, z);
			v.uv = vec2(p / (2.0f * PI), 1.0f - t / PI);
			vertices.push_back(v);

			p += delta;
		}

		t += delta;
	}

	Vector<uint> indices;

	int upLeft, upRight, downLeft, downRight;
	for(int i = 0; i < div / 2; i++) {
		for(int j = 0; j < div; j++) {
			upLeft = i * div + j;
			upRight = i * div + j + 1;

			downLeft = upLeft + div + 1;
			downRight = upRight + div + 1;

			indices.push_back(downLeft);
			indices.push_back(downRight);
			indices.push_back(upRight);

			indices.push_back(upRight);
			indices.push_back(upLeft);
			indices.push_back(downLeft);
		}
	}

	SPtr<Mesh> mesh = std::make_shared<Mesh>(vertices.size(), indices.size());
	mesh->SetVertices(vertices.data(), vertices.size());
	mesh->SetIndices(indices.data(), indices.size());
	mesh->UpdateVertexArray();

	SubMesh sm;
	sm.vertexOffset = 0;
	sm.indexOffset = 0;
	sm.indexCount = indices.size();
	mesh->SetSubMesh(&sm, 1);

	return mesh;
}

SPtr<Mesh> BaseMeshGenerator::GetCapsuleMesh()
{
	// TODO: 차후 구현
	return nullptr;
}

SPtr<Mesh> BaseMeshGenerator::GetPlaneMesh()
{
	Vector<Vertex> vertices;
	Vertex v;
	v.norm = vec3(0, 0, -1);

	v.pos = { -1.0f, -1.0f, 0.0f };
	v.uv = { 0.0f, 0.0f };
	vertices.push_back(v);
	v.pos = { -1.0f, 1.0f, 0.0f };
	v.uv = { 0.0f, 1.0f };
	vertices.push_back(v);
	v.pos = { 1.0f, 1.0f, 0.0f };
	v.uv = { 1.0f, 1.0f };
	vertices.push_back(v);
	v.pos = { 1.0f, -1.0f, 0.0f };
	v.uv = { 1.0f, 0.0f };
	vertices.push_back(v);

	Vector<uint> indices = {
		0,3,1,  2,1,3
	};

	SetNormalVector(vertices, indices);

	SPtr<Mesh> mesh = std::make_shared<Mesh>(vertices.size(), indices.size());
	mesh->SetVertices(vertices.data(), vertices.size());
	mesh->SetIndices(indices.data(), indices.size());
	mesh->UpdateVertexArray();

	SubMesh sm;
	sm.vertexOffset = 0;
	sm.indexOffset = 0;
	sm.indexCount = indices.size();
	mesh->SetSubMesh(&sm, 1);

	return mesh;
}

void BaseMeshGenerator::SetNormalVector(Vector<Vertex>& vertices, Vector<uint>& indices)
{
	for(auto& v : vertices) {
		v.norm = { 0.0f, 0.0f, 0.0f };
	}

	int numTriangles = (int)indices.size() / 3;
	for(int i = 0; i < numTriangles; i++) {
		int i0 = indices[i * 3];
		int i1 = indices[i * 3 + 1];
		int i2 = indices[i * 3 + 2];
		Vertex v0 = vertices[i0];
		Vertex v1 = vertices[i1];
		Vertex v2 = vertices[i2];

		vec3 t0;
		t0 = v1.pos - v0.pos;
		vec3 t1;
		t1 = v2.pos - v0.pos;

		vec3 n = cross(t0, t1);

		vertices[i0].norm += n;
		vertices[i1].norm += n;
		vertices[i2].norm += n;
	}

	for(auto& v : vertices) {
		v.norm = normalize(v.norm);
	}
}
