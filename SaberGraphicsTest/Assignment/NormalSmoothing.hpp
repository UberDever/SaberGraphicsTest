#pragma once
#include "../include/geometry.hpp"

// This must be Mesh methods, but I separated this out for clarity

void recalculateNormals(Mesh* mesh)
{
	assert(((mesh->vertices.size() % Model::vxCount) == 0) && "Vertices count must be divisible by 8!");
	assert(((mesh->indices.size() % 3) == 0) && "Indices count must be divisible by 3!");

	// Maximum clarity for compiler optimizer and human readability
	// Also can be parallelized with SIMD AVX/SSE (or probably with C++ parallel algos)

	// Faces must be split, for this algorithm to work (my parser automatically merge shared vertices, but than can be changed)
	for (int curTriplet = 0; curTriplet < mesh->indices.size(); curTriplet += 3)
	{
		int index0 = mesh->indices[curTriplet + 0];
		int index1 = mesh->indices[curTriplet + 1];
		int index2 = mesh->indices[curTriplet + 2];

		std::cout << "Triplet: (" << index0 << ", " << index1 << ", " << index2 << ")\n";

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

		glm::vec3 normal = glm::normalize(glm::cross(p1 - p0, p2 - p0));

		mesh->vertices[index0 * Model::vxCount + Model::vxAttr0 + Model::vxAttr1 + 0] = normal.x;
		mesh->vertices[index0 * Model::vxCount + Model::vxAttr0 + Model::vxAttr1 + 1] = normal.y;
		mesh->vertices[index0 * Model::vxCount + Model::vxAttr0 + Model::vxAttr1 + 2] = normal.z;

		mesh->vertices[index1 * Model::vxCount + Model::vxAttr0 + Model::vxAttr1 + 0] = normal.x;
		mesh->vertices[index1 * Model::vxCount + Model::vxAttr0 + Model::vxAttr1 + 1] = normal.y;
		mesh->vertices[index1 * Model::vxCount + Model::vxAttr0 + Model::vxAttr1 + 2] = normal.z;

		mesh->vertices[index2 * Model::vxCount + Model::vxAttr0 + Model::vxAttr1 + 0] = normal.x;
		mesh->vertices[index2 * Model::vxCount + Model::vxAttr0 + Model::vxAttr1 + 1] = normal.y;
		mesh->vertices[index2 * Model::vxCount + Model::vxAttr0 + Model::vxAttr1 + 2] = normal.z;
	}
}

void smoothNormals(Mesh* mesh)
{

}