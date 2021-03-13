#include "../include/renderer.hpp"

std::string Renderer::defaultProjName = "u_projection";
std::string Renderer::defaultViewName = "u_view";
std::string Renderer::defaultModelName = "u_model";
std::string Renderer::defaultIModelName = "u_iModel";

Renderer::Renderer(Shader shader) : mShader{shader}, mLightShader(lightShaderSrc)
{
    mBunch.reserve(32);
}

void Renderer::add(Model &model, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scaling)
{
    add(model, ModelInstance(model, translation, rotation, scaling));
}

void Renderer::add(Model &model, ModelInstance instance)
{
    auto instances = mBunch.find(model);
    if (instances == mBunch.end())
    {
        mBunch[model].push_back(instance);
    }
    else
    {
        instances->second.push_back(instance);
    }
}

void Renderer::removeModel(Model &model)
{
    auto instances = mBunch.find(model);
    if (instances != mBunch.end())
    {
        mBunch[model].clear();
    }
}

void Renderer::removeAll()
{
    for (auto &instances : mBunch)
        instances.second.clear();
}

void Renderer::render()
{
    for (auto &instances : mBunch)
    {
        instances.first.bind();
        int i = 0;
        for (const auto &texture : instances.first.getTextures())
        {
            mShader.bindTexture(i, texture.mId);
            i++;
        }
        for (auto &instance : instances.second)
        {
            glm::mat4 transform = instance.getTransform();
            if (mShader.hasUniform(defaultIModelName))
            {
                glm::mat4 iModel;
                iModel = glm::inverseTranspose(transform);
                mShader.setM4(defaultIModelName, iModel);
            }
            mShader.setM4(defaultModelName, transform);
            instances.first.draw();
        }
        instances.first.unbind();
    }
}

void Renderer::renderLight(std::vector<glm::vec3> &color)
{
    mLightModel.bind();
    int index = 0;
    for (auto &light : mLightInstances)
    {
        glm::mat4 transform = light.getTransform();
        mLightShader.setM4(defaultModelName, transform);
        mLightShader.setV3("u_lightColor", color[index++]);
        mLightModel.draw();
    }
    mLightModel.unbind();
}

void Renderer::addPointLight(PointLight &light)
{
    mPointLights.push_back(light);
    int index = mPointLights.size() - 1;
    mShader.setV3("u_pointLights[" + std::to_string(index) + "].position", light.pos);
    mShader.set1f("u_pointLights[" + std::to_string(index) + "].quadratic", light.attenuation.x);
    mShader.set1f("u_pointLights[" + std::to_string(index) + "].linear", light.attenuation.y);
    mShader.set1f("u_pointLights[" + std::to_string(index) + "].constant", light.attenuation.z);
    mShader.setV3("u_pointLights[" + std::to_string(index) + "].properties.ambient", light.ambient);
    mShader.setV3("u_pointLights[" + std::to_string(index) + "].properties.diffuse", light.diffuse);
    mShader.setV3("u_pointLights[" + std::to_string(index) + "].properties.specular", light.specular);
}

void Renderer::addPointLightInstance(glm::vec3 &pos)
{
    mLightInstances.push_back(ModelInstance(mLightModel, pos, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));
}

void Renderer::addDirLight(DirLight &light)
{
    mDirectionalLight = light;
    mShader.setV3("u_dirLight.direction", light.dir);
    mShader.setV3("u_dirLight.properties.ambient", light.ambient);
    mShader.setV3("u_dirLight.properties.diffuse", light.diffuse);
    mShader.setV3("u_dirLight.properties.specular", light.specular);
}
