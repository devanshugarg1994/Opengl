#include "Shader.h"
#include "Renderer.h"
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath), m_RendereID(0)
{
	ShaderProgramSource source = ParseShader(filepath);
	m_RendereID = CreateShader(source.VertexSource, source.FragmentSource);

}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendereID));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendereID));
}

void Shader::UnBind() const
{
	GLCall(glUseProgram(0));


}

 ShaderProgramSource Shader::ParseShader(std::string filepath) {
	std::fstream stream(filepath);

	enum class ShaderType {
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};
	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;

			}
		}
		else if (type != ShaderType::NONE) {
			ss[(int)type] << line << "\n";
		}

	}
	return { ss[0].str(), ss[1].str() };

}
unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
	const unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));


	// Error Handling of Shader
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

	if (result == GL_FALSE) {
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to Compile " << ((type == GL_VERTEX_SHADER) ? "VertexShader" : "fragmentShader") << message;
		GLCall(glDeleteShader(id));

		return 0;
	}

	return id;
}
 unsigned int  Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	GLCall(unsigned int program = glCreateProgram());
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));


	GLCall(glDeleteShader(vs));  // Delete intermediates object created while compiling shader source cod)e
	GLCall(glDeleteShader(fs));

	return program;
}
 void Shader::SetUniform1i(const std::string& name, int value)
 {
	 GLCall(glUniform1i(GetUniformLocation(name), value));

 }
 void Shader::SetUniform1f(const std::string& name, float value)
 {
	 GLCall(glUniform1f(GetUniformLocation(name), value));


 }
 void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));

}

 void Shader::SetUnifromMat4f(const std::string& name, const glm::mat4& matrix)
 {
	 GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));

 }

 int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
		return m_UniformLocationCache[name];
	}
	GLCall(int location = glGetUniformLocation(m_RendereID, name.c_str()));
	if (location == -1) {
		std::cout << "Warning: uniform ' " << name << "'does not exist" << std::endl;
	}

	m_UniformLocationCache[name] = location;
	return location;

}
