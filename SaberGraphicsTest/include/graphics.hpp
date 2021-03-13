#ifndef SABERGRAPHICSTEST_GRAPHICS_H
#define SABERGRAPHICSTEST_GRAPHICS_H

#include "core.hpp"

struct Texture
{
	int mW, mH;
	GLuint mId;
	uint8_t* mData;

	Texture() = default;
	Texture(std::string path)
	{
		load(path);
	}

	void load(std::string path)
	{
		glGenTextures(1, &mId);
		int nrComponents;
		unsigned char* data = stbi_load(path.c_str(), &mW, &mH, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, mId);
			glTexImage2D(GL_TEXTURE_2D, 0, format, mW, mH, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
			exit(EXIT_FAILURE);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}
};

class Camera
{
public:
	glm::mat4 projection, view;
	glm::vec3 position, up, forward, right;
	glm::vec2 rotation;
	float sensitivity, speed;

public:
	Camera() : position(0, 0, 0), right(1, 0, 0), up(0, 1, 0), forward(0, 0, 1), rotation(-M_PI_2, 0), speed(0.1f), sensitivity(0.01f) {}

	void setProjection(float fov, float aspect, float near, float far) { projection = glm::perspective(fov, aspect, near, far); }
	void setOrtho(float left, float right, float bottom, float top) { projection = glm::ortho(left, right, bottom, top, 0.1f, 1000.f); }
	void setView(glm::vec3& eye, glm::vec3& center, glm::vec3& up) { view = glm::lookAt(eye, eye + center, up); }
	void setView() { view = glm::lookAt(position, position + forward, up); }
};

struct Mesh
{
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	Mesh(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices) : vertices(vertices), indices(indices) {}
};
typedef unsigned int ModelId_t;
struct ModelInfo
{
	ModelId_t id;
	GLuint vao;
	GLsizei idx_count;
	GLuint drawMode;

	ModelInfo() : drawMode(GL_TRIANGLES), vao(0), idx_count(0) {}
};

class Model
{
	static ModelId_t registeredModelsCount;

	ModelInfo info;
	std::vector<Texture> mTextures;

public:
	static constexpr auto vxAttr0 = 3;
	static constexpr auto vxAttr1 = 2;
	static constexpr auto vxAttr2 = 3;
	static constexpr auto vxCount = vxAttr0 + vxAttr1 + vxAttr2;

	Model() = default;
	Model(Mesh& mesh);
	~Model() { registeredModelsCount--; }

	ModelId_t getId() const { return info.id; }

	bool operator==(const Model& rhs) const
	{
		return rhs.getId() == info.id;
	}

	void bind() const
	{
		glBindVertexArray(info.vao);
	}

	void unbind() const
	{
		glBindVertexArray(0);
	}

	void draw() const
	{
		glDrawElements(info.drawMode, info.idx_count, GL_UNSIGNED_INT, 0);
	}

	void setDrawMode(int drawMode) { info.drawMode = drawMode; }

	void addTexture(Texture& tex) { mTextures.push_back(tex); }
	const std::vector<Texture>& getTextures() const { return mTextures; }
};

namespace std
{
	template <>
	struct hash<Model>
	{
		std::size_t operator()(const Model& key) const
		{
			return std::hash<ModelId_t>()(key.getId());
		}
	};
} // namespace std

class ModelInstance
{
	Model& mModel;
	glm::vec3 mTranslation;
	glm::vec3 mRotation;
	glm::vec3 mScaling;
	bool mIsActive;

public:
	ModelInstance(Model& model, glm::vec3 t = { 0, 0, 0 }, glm::vec3 r = { 0, 0, 0 }, glm::vec3 s = { 1, 1, 1 })
		: mIsActive(true), mTranslation(t), mRotation(r), mScaling(s), mModel(model)
	{
	}

	glm::mat4 getTransform()
	{
		glm::mat4 scaling = glm::scale(glm::mat4(1), mScaling);
		glm::mat4 rotation = glm::eulerAngleXYZ(mRotation.x, mRotation.y, mRotation.z);
		glm::mat4 translation = glm::translate(glm::mat4(1), mTranslation);
		return translation * rotation * scaling;
	}

	bool& isActive() { return mIsActive; }
};

struct Light
{
	// Parameters, passed to shader
	glm::vec3 ambient, diffuse, specular;
};

struct DirLight : public Light
{
	glm::vec3 dir;
};

struct PointLight : public Light
{
	glm::vec3 pos;
	glm::vec3 attenuation; // x * d^2 + y * d + z * d^0
};

#endif