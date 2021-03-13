#ifndef SABERGRAPHICSTEST_SHADER_H
#define SABERGRAPHICSTEST_SHADER_H

#include "core.hpp"
#include "graphics.hpp"

struct Uniform
{
    std::string name;
    GLint location;

    Uniform() : name(""), location(-1) {}
};

class Shader
{
    GLuint mId;
    std::unordered_map<std::string, Uniform> mUniforms;
    std::vector<GLuint> mTextureSlots;

    static std::string load(std::string &path);
    static void check_error(GLuint id);
    static void compile(std::string &path, GLuint id, char const **stream, uint strings = 1);

public:
    Shader() = default;
    Shader(const Shader &) = default;
    Shader(std::string vs, std::string fs);
    Shader(std::string src);

    void bind() { glUseProgram(mId); }
    void unbind() { glUseProgram(0); }

    GLint createUniform(std::string &uniformName);
    GLuint getUniformLocation(std::string &name) { return glGetUniformLocation(mId, name.c_str()); }
    bool hasUniform(std::string &name)
    {
        if (mUniforms.find(name) == mUniforms.end())
        {
            if (getUniformLocation(name) != -1)
            {
                createUniform(name);
                return true;
            }
            else
                return false;
        }
        else
            return true;
    }

    void findSamplersLocation(std::vector<std::string> &textureNames)
    {
        for (auto &it : textureNames)
        {
            mTextureSlots.push_back(getUniformLocation(it));
        }
    }

    void bindTexture(GLint slot, GLuint id)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, id);
        glUniform1i(mTextureSlots[slot], slot);
    };

    void set1i(std::string uniformName, int i);
    void set1f(std::string uniformName, float f);

    void setV3(std::string uniformName, glm::vec3 v);
    void setV3(std::string uniformName, float x, float y, float z);

    void setM4(std::string uniformName, glm::mat4 &m);
};

#endif