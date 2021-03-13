#ifndef SABERGRAPHICSTEST_RENDERER_H
#define SABERGRAPHICSTEST_RENDERER_H

#include "graphics.hpp"
#include "shader.hpp"

class Renderer
{
	static constexpr const char* const lightShaderSrc = "shaders/light.glsl";

	Shader mShader;
	std::unordered_map<Model, std::vector<ModelInstance>> mBunch;

	Shader mLightShader;
	Model mLightModel;
	Light mDirectionalLight;
	std::vector<Light> mPointLights;
	std::vector<ModelInstance> mLightInstances;

public:
	static std::string defaultProjName;
	static std::string defaultViewName;
	static std::string defaultModelName;
	static std::string defaultIModelName;

public:
	Renderer(Shader shader);

	void add(Model& model, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scaling);
	void add(Model& model, ModelInstance instance);

	void removeModel(Model& model);
	void removeAll();

	void render();
	void renderLight(std::vector<glm::vec3>& color);

	Shader& mainShader() { return mShader; }
	Shader& lightShader() { return mLightShader; }

	void setLightModel(Model& model) { mLightModel = model; };

	void addPointLight(PointLight& light);
	void addPointLightInstance(glm::vec3& pos);
	void clearPointLights()
	{
		mPointLights.clear();
		mLightInstances.clear();
	}
	int getPointLightCount() { return mPointLights.size(); }
	void addDirLight(DirLight& light);
};

#endif