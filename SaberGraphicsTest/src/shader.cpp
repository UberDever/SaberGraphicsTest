#include "../include/shader.hpp"

std::string Shader::load(std::string& path)
{
	FILE* shader_file = NULL;
	char* shader_stream = NULL;
	auto err = fopen_s(&shader_file, path.c_str(), "r");
	if (shader_file)
	{
		fseek(shader_file, 0, SEEK_END);
		uint length = ftell(shader_file);
		fseek(shader_file, 0, SEEK_SET);
		shader_stream = (char*)malloc((length + 1) * sizeof(char));
		memset(shader_stream, 0, (length + 1) * sizeof(char));
		if (shader_stream)
		{
			fread(shader_stream, 1, length, shader_file);
		}
		shader_stream[length] = 0;
		fclose(shader_file);
		std::string str = std::string(shader_stream);
		free(shader_stream);
		return std::move(str);
	}
	else
	{
		std::cout << "Cannot load shader " << path << std::endl;
		exit(EXIT_FAILURE);
		return std::string();
	}
}

void Shader::check_error(GLuint id)
{
	GLint result;
	int log_len;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_len);
	if (log_len > 0)
	{
		char* error_message = new char[log_len + 1];
		memset(error_message, 0, sizeof(char) * (log_len + 1));
		glGetShaderInfoLog(id, log_len, NULL, &error_message[0]);
		std::cout << "Status: " << result << std::endl;
		std::cout << "Message: " << error_message << std::endl;
		delete[] error_message;
		exit(EXIT_FAILURE);
	}
}

void Shader::compile(std::string& path, GLuint id, char const** stream, uint strings)
{
	std::cout << "Compiling shader " << path << std::endl;
	glShaderSource(id, strings, stream, NULL);
	glCompileShader(id);

	check_error(id);
}

Shader::Shader(std::string vs, std::string fs)
{
	// Create the shaders
	GLuint vsId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fsId = glCreateShader(GL_FRAGMENT_SHADER);

	std::cout << "Loading shaders" << std::endl;
	std::string vsStream = load(vs);
	std::string fsStream = load(fs);

	std::cout << "Compiling shaders" << std::endl;
	const char* vsStr = vsStream.c_str();
	const char* fsStr = fsStream.c_str();
	compile(vs, vsId, &vsStr);
	compile(fs, fsId, &fsStr);

	// Link the program
	std::cout << "Linking program" << std::endl;
	mId = glCreateProgram();
	glAttachShader(mId, vsId);
	glAttachShader(mId, fsId);
	glLinkProgram(mId);

	// Check the program
	check_error(mId);

	glDetachShader(mId, vsId);
	glDetachShader(mId, fsId);

	glDeleteShader(vsId);
	glDeleteShader(fsId);
}

Shader::Shader(std::string src)
{
	// Create the shaders
	GLuint vsId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fsId = glCreateShader(GL_FRAGMENT_SHADER);

	std::cout << "Loading shaders" << std::endl;
	std::string stream = load(src);

	std::cout << "Compiling shaders" << std::endl;
	const char* strVs[2] = { "#version 330 core\n#define COMPILE_VS\n", stream.c_str() };
	compile(src, vsId, strVs, 2);
	const char* strFs[2] = { "#version 330 core\n#define COMPILE_FS\n", stream.c_str() };
	compile(src, fsId, strFs, 2);

	// Link the program
	std::cout << "Linking program" << std::endl;
	mId = glCreateProgram();
	glAttachShader(mId, vsId);
	glAttachShader(mId, fsId);
	glLinkProgram(mId);

	// Check the program
	check_error(mId);

	glDetachShader(mId, vsId);
	glDetachShader(mId, fsId);

	glDeleteShader(vsId);
	glDeleteShader(fsId);
}

void Shader::set1i(std::string uniformName, int i)
{
	if (mUniforms.find(uniformName) != mUniforms.end())
	{
		glUniform1i(mUniforms[uniformName].location, i);
	}
	else
	{
		Uniform newUniform;
		newUniform.name = uniformName;
		newUniform.location = getUniformLocation(uniformName);
		mUniforms[uniformName] = newUniform;
		glUniform1i(newUniform.location, i);
	}
}

GLint Shader::createUniform(std::string& uniformName)
{
	Uniform newUniform;
	newUniform.name = uniformName;
	newUniform.location = getUniformLocation(uniformName);
	if (newUniform.location == -1)
	{
		std::cout << "Uniform with name " << uniformName << " not found" << std::endl;
	}
	mUniforms[uniformName] = newUniform;
	return newUniform.location;
}

void Shader::set1f(std::string uniformName, float f)
{
	if (mUniforms.find(uniformName) != mUniforms.end())
	{
		glUniform1f(mUniforms[uniformName].location, f);
	}
	else
	{
		glUniform1f(createUniform(uniformName), f);
	}
}

void Shader::setV3(std::string uniformName, float x, float y, float z)
{
	if (mUniforms.find(uniformName) != mUniforms.end())
	{
		glUniform3f(mUniforms[uniformName].location, x, y, z);
	}
	else
	{
		GLint loc = createUniform(uniformName);
		glUniform3f(loc, x, y, z);
	}
}

void Shader::setV3(std::string uniformName, glm::vec3 v)
{
	if (mUniforms.find(uniformName) != mUniforms.end())
	{
		glUniform3fv(mUniforms[uniformName].location, 1, &v[0]);
	}
	else
	{
		glUniform3fv(createUniform(uniformName), 1, &v[0]);
	}
}

void Shader::setM4(std::string uniformName, glm::mat4& m)
{
	if (mUniforms.find(uniformName) != mUniforms.end())
	{
		glUniformMatrix4fv(mUniforms[uniformName].location, 1, GL_FALSE, (const GLfloat*)glm::value_ptr(m));
	}
	else
	{
		glUniformMatrix4fv(createUniform(uniformName), 1, GL_FALSE, (const GLfloat*)glm::value_ptr(m));
	}
}