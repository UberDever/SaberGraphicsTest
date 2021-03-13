#pragma warning(disable : 4996)

#ifndef SABERGRAPHICSTEST_GEOMETRY_H
#define SABERGRAPHICSTEST_GEOMETRY_H

#include "core.hpp"
#include "graphics.hpp"

static inline Mesh createSphere(const GLuint numberSlices, const GLfloat radius = 1.0f)
{
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	GLuint numberParallels = numberSlices / 2;
	GLuint numberVertices = (numberParallels + 1u) * (numberSlices + 1u);
	GLuint numberIndices = numberParallels * numberSlices * 6u;

	assert(radius > 0u);
	assert(numberSlices >= 3u);

	GLfloat angleStep = M_PI * 2.f / numberSlices;

	for (GLuint i = 0u; i < numberParallels + 1u; i++)
	{
		for (GLuint j = 0u; j < numberSlices + 1u; j++)
		{
			GLuint index = (i * (numberSlices + 1u) + j);
			GLfloat x, y, z;
			x = radius * sinf(angleStep * (GLfloat)i) * sinf(angleStep * (GLfloat)j);
			vertices.push_back(x);
			y = radius * cosf(angleStep * (GLfloat)i);
			vertices.push_back(y);
			z = radius * sinf(angleStep * (GLfloat)i) * cosf(angleStep * (GLfloat)j);
			vertices.push_back(z);
			vertices.push_back((GLfloat)j / (GLfloat)numberSlices); // tex coords
			vertices.push_back(((GLfloat)i / numberParallels));
			vertices.push_back(x / radius); // normals
			vertices.push_back(y / radius);
			vertices.push_back(z / radius);
		}
	}

	GLuint indexIndices = 0u;
	for (GLuint i = 0u; i < numberParallels; i++)
	{
		for (GLuint j = 0u; j < numberSlices; j++)
		{
			indices.push_back(i * (numberSlices + 1u) + j);
			indices.push_back((i + 1u) * (numberSlices + 1u) + j);
			indices.push_back((i + 1u) * (numberSlices + 1u) + (j + 1u));

			indices.push_back(i * (numberSlices + 1u) + j);
			indices.push_back((i + 1u) * (numberSlices + 1u) + (j + 1u));
			indices.push_back(i * (numberSlices + 1u) + (j + 1u));
		}
	}

	return std::move(Mesh(vertices, indices));
}

static Mesh createCube(const GLfloat side, glm::vec3 color)
{
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
	int sign[3] = { 1, 1, 1 };

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				for (int l = 0; l < 3; l++)
				{
					GLfloat _side = side * sign[l];
					vertices.push_back(_side);
				}
				vertices.push_back(0);
				vertices.push_back(0);
				vertices.push_back(color.x);
				vertices.push_back(color.y);
				vertices.push_back(color.z);
				sign[2] = -1;
			}
			sign[1] = -1;
			sign[2] = 1;
		}
		sign[0] = -1;
		sign[1] = 1;
		sign[2] = 1;
	}

	GLuint indices_[] = { 1, 0, 2, 3, 1, 2, 7, 1, 3, 7, 5, 1, 2, 0, 6, 6, 0, 4, 7, 6, 5, 5, 6, 4, 5, 0, 1, 5, 4, 0, 7, 3, 2, 7, 2, 6 };
	for (int i = 0; i < (sizeof(indices_) / sizeof(*indices_)); i++)
	{
		indices.push_back(indices_[i]);
	}
	return std::move(Mesh(vertices, indices));
}

static Mesh createHyper(float height, float a, float b, uint offset, uint num_vertices, glm::vec3 colorCenter0, glm::vec3 colorCenter1, glm::vec3 colorSide)
{
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
	float phi = 0;
	float d_phi = 2 * M_PI / (num_vertices - 0);
	//float color1[3] = {0.1294, 0.1294, 0.3};
	//float color1[3] = {148 / 255.f, 18 / 255.f, 29 / 255.f};
	//float color2[3] = {2 / 255.f, 40 / 255.f, 64 / 255.f};
	for (uint i = 0; i < Model::vxCount - 3; i++) // push center 1 coordinates
		vertices.push_back(0);
	for (int i = 0; i < 3; i++)
	{
		vertices.push_back(colorCenter0[i]);
	}
	for (uint i = 0; i < Model::vxCount - 3; i++) // push center 2 coordinates
		vertices.push_back(0);
	for (int i = 0; i < 3; i++)
	{
		vertices.push_back(colorCenter1[i]);
	}
	vertices[2] = height;
	vertices[Model::vxCount + 2] = -height;
	for (int c = 0; c <= 3; c++)
	{
		phi = 0;
		for (uint i = 0; i <= num_vertices; phi += d_phi, i++)
		{
			float x = a * cosf(phi);
			vertices.push_back(x);
			float y = b * sinf(phi);
			vertices.push_back(y);
			vertices.push_back(height);
			float uv[2] = {};
			uint n = num_vertices;
			if (height > 0)
			{
				uv[0] = 1 - (float)((i + offset)) / n;
				if (uv[0] < 0)
					uv[0] += 1;
				uv[1] = 1;
			}
			else
			{
				uv[0] = 1 - (float)(i) / n;
				if (uv[0] < 0)
					uv[0] += 1;
				uv[1] = 0;
			}
			vertices.push_back(uv[0]);
			vertices.push_back(uv[1]);
			vertices.push_back(colorSide[0]);
			vertices.push_back(colorSide[1]);
			vertices.push_back(colorSide[2]);
		}
		height *= -1.f;
	}
	num_vertices += 1;
	for (uint i = 2; i < num_vertices + 2; i++)
	{
		uint j = i + num_vertices + offset;
		if (j >= 2 * num_vertices + 2)
			j -= num_vertices;
		uint i_inc = i + 1;
		uint j_inc = j + 1;
		if (i_inc >= num_vertices + 2)
			i_inc = 2;
		if (j_inc >= 2 * num_vertices + 2)
			j_inc = num_vertices + 2;
		indices.push_back(i);
		indices.push_back(j);
		indices.push_back(i_inc);
		indices.push_back(i_inc);
		indices.push_back(j);
		indices.push_back(j_inc);
	}
	uint center = 0;
	for (uint i = 2 * num_vertices + 2; i < 3 * num_vertices + 2; i++)
	{
		indices.push_back(i);
		uint tmp = i + 1;
		if (tmp >= 3 * num_vertices + 2)
			tmp = 2 * num_vertices + 2;
		indices.push_back(tmp);
		indices.push_back(center);
	}
	center = 1;
	for (uint i = 3 * num_vertices + 2; i < 4 * num_vertices + 2; i++)
	{
		indices.push_back(i);
		indices.push_back(center);
		uint tmp = i + 1;
		if (tmp >= 4 * num_vertices + 2)
			tmp = 3 * num_vertices + 2;
		indices.push_back(tmp);
	}
	return std::move(Mesh(vertices, indices));
}

static Mesh createCone(float height, float a, float b, uint num_vertices, glm::vec3 colorCenter0, glm::vec3 colorCenter1, glm::vec3 colorSide)
{
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
	float phi = 0;
	float d_phi = 2 * M_PI / (num_vertices - 1);
	for (uint i = 0; i < Model::vxCount - 3; i++) // push center 1 coordinates
		vertices.push_back(0);
	for (int i = 0; i < 3; i++)
	{
		vertices.push_back(colorCenter0[i]);
	}
	for (uint i = 0; i < Model::vxCount - 3; i++) // push center 2 coordinates
		vertices.push_back(0);
	for (int i = 0; i < 3; i++)
	{
		vertices.push_back(colorCenter1[i]);
	}
	vertices[2] = -height;
	vertices[Model::vxCount + 2] = 0;
	for (int c = 0; c <= 1; c++)
	{
		phi = 0;
		for (uint i = 0; i < num_vertices; phi += d_phi, i++)
		{
			float x = a * cosf(phi);
			vertices.push_back(x);
			float y = b * sinf(phi);
			vertices.push_back(y);
			vertices.push_back(0);

			float uv[2] = {};
			uint n = num_vertices + 1;
			uv[0] = 1 - (float)(i) / n;
			if (height > 0)
			{
				uv[1] = 1;
			}
			else
			{
				uv[1] = 0;
			}
			vertices.push_back(uv[0]);
			vertices.push_back(uv[1]);
			vertices.push_back(colorSide[0]);
			vertices.push_back(colorSide[1]);
			vertices.push_back(colorSide[2]);
		}
	}
	uint center = 0;
	for (uint i = 2; i < num_vertices + 2; i++)
	{
		indices.push_back(i);
		indices.push_back(center);
		uint tmp = i + 1;
		if (tmp >= num_vertices + 2)
			tmp = 2;
		indices.push_back(tmp);
	}
	center = 1;
	for (uint i = num_vertices + 2; i < 2 * num_vertices + 2; i++)
	{
		indices.push_back(i);
		uint tmp = i + 1;
		if (tmp >= 2 * num_vertices + 2)
			tmp = num_vertices + 2;
		indices.push_back(tmp);
		indices.push_back(center);
	}
	return std::move(Mesh(vertices, indices));
}

static inline void fillVertex(std::vector<GLfloat>& vertices, glm::vec3 pos, glm::vec2 tex, glm::vec3 norm)
{
	vertices.push_back(pos.x);
	vertices.push_back(pos.y);
	vertices.push_back(pos.z);
	vertices.push_back(tex.x);
	vertices.push_back(tex.y);
	vertices.push_back(norm.x);
	vertices.push_back(norm.y);
	vertices.push_back(norm.z);
}

static Mesh createPyramid(float baseLen, float height)
{
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
	glm::vec3 top = { 0, height, 0 };
	glm::vec3 p0 = { -baseLen / 2, 0, -baseLen / 2 };
	glm::vec3 p1 = { baseLen / 2, 0, -baseLen / 2 };
	glm::vec3 p2 = { baseLen / 2, 0, baseLen / 2 };
	glm::vec3 p3 = { -baseLen / 2, 0, baseLen / 2 };
	glm::vec3 normSide0 = glm::normalize(glm::cross(top - p3, p0 - p3));
	glm::vec3 normSide1 = glm::normalize(glm::cross(top - p0, p1 - p0));
	glm::vec3 normSide2 = glm::normalize(glm::cross(top - p1, p2 - p1));
	glm::vec3 normSide3 = glm::normalize(glm::cross(top - p2, p3 - p2));
	glm::vec3 normBottom = glm::normalize(glm::cross(p3 - p0, p1 - p0));

	fillVertex(vertices, p0, {}, normSide0);
	fillVertex(vertices, p3, {}, normSide0);
	fillVertex(vertices, top, {}, normSide0);

	fillVertex(vertices, p1, {}, normSide1);
	fillVertex(vertices, p0, {}, normSide1);
	fillVertex(vertices, top, {}, normSide1);

	fillVertex(vertices, p2, {}, normSide2);
	fillVertex(vertices, p1, {}, normSide2);
	fillVertex(vertices, top, {}, normSide2);

	fillVertex(vertices, p3, {}, normSide3);
	fillVertex(vertices, p2, {}, normSide3);
	fillVertex(vertices, top, {}, normSide3);

	fillVertex(vertices, p0, {}, normBottom);
	fillVertex(vertices, p1, {}, normBottom);
	fillVertex(vertices, p2, {}, normBottom);
	fillVertex(vertices, p0, {}, normBottom);
	fillVertex(vertices, p2, {}, normBottom);
	fillVertex(vertices, p3, {}, normBottom);

	for (int i = 0; i < 18; i++)
		indices.push_back(i);

	return std::move(Mesh(vertices, indices));
}

/*
	Exported from C project, code needs refactoring to C++
*/

typedef std::unordered_map<std::string, GLuint> mStringToGLuint;
typedef std::vector<GLfloat> vGLfloat;
typedef std::vector<GLuint> vGLuint;

static void
obj_parse_face(char* face_name, mStringToGLuint* table_faces, uint* cur_id,
	vGLfloat* vertices, vGLuint* indices, vGLfloat positions, vGLfloat tex_coords, vGLfloat normals, bool preserveSplitFaces)
{
	auto index = table_faces->find(face_name);
	if (index != table_faces->end() && !preserveSplitFaces)
	{
		indices->push_back(index->second);
	}
	else
	{
		(*table_faces)[face_name] = *cur_id;
		indices->push_back(*cur_id);
		(*cur_id)++;

		int face_indices[3] = { -1, -1, -1 };
		char* ind = strtok(face_name, "/");
		for (uint i = 0; i < 3 && ind; i++)
		{
			face_indices[i] = atoi(ind) - 1;
			ind = strtok(NULL, "/");
		}
		//printf("%d %d %d\n", face_indices[0], face_indices[1], face_indices[2]);
		vertices->push_back(positions[face_indices[0] * 3]);
		vertices->push_back(positions[face_indices[0] * 3 + 1]);
		vertices->push_back(positions[face_indices[0] * 3 + 2]);
		if (face_indices[1] >= 0)
		{
			vertices->push_back(tex_coords[face_indices[1] * 2 + 0]);
			vertices->push_back(tex_coords[face_indices[1] * 2 + 1]);
		}
		else
		{
			vertices->push_back(0);
			vertices->push_back(0);
		}
		if (face_indices[2] >= 0)
		{
			vertices->push_back(normals[face_indices[2] * 3]);
			vertices->push_back(normals[face_indices[2] * 3 + 1]);
			vertices->push_back(normals[face_indices[2] * 3 + 2]);
		}
		else
		{
			vertices->push_back(0);
			vertices->push_back(0);
			vertices->push_back(0);
		}
	}
}

#define SKIP_LINE(c, file)            \
  do                                  \
  {                                   \
    while ((c) != '\n' && (c) != EOF) \
      c = fgetc(file);                \
  } while (0)


#define obj_line_size 100
static Mesh obj_parse(const char* path, bool preserveSplitFaces = false)
{
	FILE* obj_file = fopen(path, "r");
	if (!obj_file)
	{
		std::cout << "Cannot open " << path << " obj file" << std::endl;
		exit(EXIT_FAILURE);
	}

	uint v_count = 0;
	uint vt_count = 0;
	uint vn_count = 0;
	uint ind_count = 0;

	vGLfloat vertices;
	vGLuint indices;
	char ch = 0;
	do
	{
		ch = fgetc(obj_file);

		switch (ch)
		{
		case EOF:
			break;

		case 'v':
			ch = fgetc(obj_file);
			switch (ch)
			{
			case ' ':
				v_count++;
				SKIP_LINE(ch, obj_file);
				break;

			case 't':
				vt_count++;
				SKIP_LINE(ch, obj_file);
				break;

			case 'n':
				vn_count++;
				SKIP_LINE(ch, obj_file);
				break;

			default:
				std::cout << "Unsupported vertex type, aborting";
			}
			break;

		case 'f':
			ind_count++;
			SKIP_LINE(ch, obj_file);
			break;

		default:
			SKIP_LINE(ch, obj_file);
			break;
		}

	} while (ch != EOF);

	mStringToGLuint table_faces;

	vGLfloat positions, tex_coords, normals;
	uint cur_id = 0;

	fseek(obj_file, 0, SEEK_SET);

	do
	{
		glm::vec3 v;
		ch = fgetc(obj_file);

		switch (ch)
		{
		case EOF:
			break;

		case '#':
			SKIP_LINE(ch, obj_file);
			break;

		case 'v':
			ch = fgetc(obj_file);
			switch (ch)
			{
			case ' ':
				fscanf(obj_file, "%f %f %f\n", &v.x, &v.y, &v.z);
				positions.push_back(v.x);
				positions.push_back(v.y);
				positions.push_back(v.z);
				break;

			case 't':
				fscanf(obj_file, " %f %f\n", &v.x, &v.y);
				v.y = 1 - v.y;
				//v.x = 1 - v.x;
				tex_coords.push_back(v.x);
				tex_coords.push_back(v.y);
				break;

			case 'n':
				fscanf(obj_file, " %f %f %f\n", &v.x, &v.y, &v.z);
				normals.push_back(v.x);
				normals.push_back(v.y);
				normals.push_back(v.z);
				break;

			default:
				std::cout << "Unsupported vertex type, aborting";
			}
			break;

		case 'f':
		{
			char face1[16] = { 0 };
			char face2[16] = { 0 };
			char face3[16] = { 0 };
			fscanf(obj_file, "%15s", face1);
			fscanf(obj_file, "%15s", face2);
			fscanf(obj_file, "%15s", face3);
			obj_parse_face(face1, &table_faces, &cur_id, &vertices, &indices, positions, tex_coords, normals, preserveSplitFaces);
			obj_parse_face(face2, &table_faces, &cur_id, &vertices, &indices, positions, tex_coords, normals, preserveSplitFaces);
			obj_parse_face(face3, &table_faces, &cur_id, &vertices, &indices, positions, tex_coords, normals, preserveSplitFaces);
		}
		break;

		default:
			SKIP_LINE(ch, obj_file);
			break;
		}

	} while (ch != EOF);

	// for (uint i = 0; i < vertices->size; i+=8)
	// {
	//    debug("p: %f %f %f t: %f %f n: %f %f %f", vertices->data[i], vertices->data[i + 1], vertices->data[i + 2], vertices->data[i + 3],
	//                                              vertices->data[i + 4], vertices->data[i + 5], vertices->data[i + 6], vertices->data[i + 7]);
	// }
	// for (uint i = 0; i < indices->size; i++)
	// {
	//    printf("%d ", indices->data[i]);
	// }
	// debug("\n%d %d", vertices->size, indices->size);

	fclose(obj_file);
	return Mesh(vertices, indices);
}

#endif