#pragma once
#include "../include/geometry.hpp"

// This must be Mesh methods, but I separated this out for clarity

// Smoothing normals, based on face area and angle of current vertex in current face
// Can be tweaked to averaging without weights too
template <bool isWeighted>
void smoothNormals(Mesh* mesh)
{
	assert(((mesh->vertices.size() % Model::vxCount) == 0) && "Vertices count must be divisible by 8!");
	assert(((mesh->indices.size() % 3) == 0) && "Indices count must be divisible by 3!");

	// Maps current vertex position to corresponding vertex normal => hence, multiple vertices with same positions
	// will have one normal
	std::unordered_map<glm::vec3, glm::vec3> normalBuffer;

	for (int curTriplet = 0; curTriplet < mesh->indices.size(); curTriplet += 3)
	{
		int index0 = mesh->indices[curTriplet + 0];
		int index1 = mesh->indices[curTriplet + 1];
		int index2 = mesh->indices[curTriplet + 2];

		glm::vec3 p0;
		p0.x = mesh->vertices[index0 * Model::vxCount + 0];
		p0.y = mesh->vertices[index0 * Model::vxCount + 1];
		p0.z = mesh->vertices[index0 * Model::vxCount + 2];

		glm::vec3 p1;
		p1.x = mesh->vertices[index1 * Model::vxCount + 0];
		p1.y = mesh->vertices[index1 * Model::vxCount + 1];
		p1.z = mesh->vertices[index1 * Model::vxCount + 2];

		glm::vec3 p2;
		p2.x = mesh->vertices[index2 * Model::vxCount + 0];
		p2.y = mesh->vertices[index2 * Model::vxCount + 1];
		p2.z = mesh->vertices[index2 * Model::vxCount + 2];

		if constexpr (isWeighted)
		{
			// Corresponding sides of triangle face
			glm::vec3 side01 = glm::normalize(p1 - p0);
			glm::vec3 side02 = glm::normalize(p2 - p0);
			glm::vec3 side12 = glm::normalize(p2 - p1);

			// Face weighting - simply no need for normalization
			glm::vec3 normal = glm::cross(p1 - p0, p2 - p0);

			// Angle weighting
			float alpha = glm::angle(side01, side02);
			float beta = glm::angle(-side01, side12);
			float gamma = glm::angle(-side02, -side12);

			normalBuffer[p0] += normal * alpha;
			normalBuffer[p1] += normal * beta;
			normalBuffer[p2] += normal * gamma;
		}
		else
		{
			// Simple face normal
			glm::vec3 normal = glm::normalize(glm::cross(p1 - p0, p2 - p0));

			normalBuffer[p0] += normal;
			normalBuffer[p1] += normal;
			normalBuffer[p2] += normal;
		}
	}

	// Getting corresponding normal from current position
	// All same positions will have exact one smoothed normal
	for (int curVertex = 0; curVertex < mesh->vertices.size(); curVertex += Model::vxCount)
	{
		glm::vec3 p;
		p.x = mesh->vertices[curVertex + 0];
		p.y = mesh->vertices[curVertex + 1];
		p.z = mesh->vertices[curVertex + 2];

		glm::vec3 normal = glm::normalize(normalBuffer[p]);
		mesh->vertices[curVertex + Model::vxAttr0 + Model::vxAttr1 + 0] = normal.x;
		mesh->vertices[curVertex + Model::vxAttr0 + Model::vxAttr1 + 1] = normal.y;
		mesh->vertices[curVertex + Model::vxAttr0 + Model::vxAttr1 + 2] = normal.z;
	}
}