#ifndef SHADER_H
#define SHADER_H

//#include <glad/glad.h>
#include "../library/include/glad/glad.h"


//#include <glm/glm.hpp>
#include "../library/include/glm/glm.hpp"

using namespace glm;
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

class Shader {
private:
	GLuint program;				// ���������ƣ�ÿ����ɫ��Ҳ��Ψһ�ĵ�Ԫ��֮ƥ��
public:
	Shader(string vertexPath, string fragmentPath) {
		string vertexCode;
		string fragmentCode;
		ifstream vShaderFile(vertexPath);
		ifstream fShaderFile(fragmentPath);
		stringstream vShaderStream, fShaderStream;

		// ��ȡ�ļ��е���ɫ��
		string line;
		while (!vShaderFile.eof()) {
			getline(vShaderFile, line);
			vShaderStream << line + '\n';
		}
		while (!fShaderFile.eof()) {
			getline(fShaderFile, line);
			fShaderStream << line + '\n';
		}

		// ��������ת��Ϊstring
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		// ������ɫ��
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vShaderCode, NULL);
		glCompileShader(vertexShader);
		checkCompileErrors(vertexShader, "VERTEX");
		// Ƭ����ɫ��
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
		glCompileShader(fragmentShader);
		checkCompileErrors(fragmentShader, "FRAGMENT");
		// ��ɫ������
		program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);
		checkCompileErrors(program, "PROGRAM");
		// ��ɫ�����ӳ����ɾ��
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	Shader(Shader* shader) {
		program = shader->GetProgram();
	}

	~Shader() {
		glDeleteProgram(program);
	}

	GLuint GetProgram() {
		return program;
	}
	// ����ɫ��
	void Bind() {
		glUseProgram(program);
	}
	// �����ɫ��
	void Unbind() {
		glUseProgram(0);
	}

	void SetInt(const char* name, int val) {
		glUniform1i(GetLocation(name), val);
	}

	void SetFloat(const char* name, float val) {
		glUniform1f(GetLocation(name), val);
	}

	void SetVec3(const char* name, vec3 val) {
		glUniform3fv(GetLocation(name), 1, &val[0]);
	}

	void SetMat3(const char* name, mat3 val) {
		glUniformMatrix3fv(GetLocation(name), 1, GL_FALSE, &val[0][0]);
	}

	void SetMat4(const char* name, mat4 val) {
		glUniformMatrix4fv(GetLocation(name), 1, GL_FALSE, &val[0][0]);
	}
private:
	// ��������ɫ����������û�г���
	void checkCompileErrors(GLuint shader, string type) {
		GLint success;
		GLchar infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << endl;
			}
		}
	}
	// ��ȡ��ɫ���ڲ������ĵ�ַ
	GLuint GetLocation(const char* name) {
		GLint location = glGetUniformLocation(program, name);

		if (location == -1) {
			cout << "Uniform '" << name << "' not defined" << endl;
		}

		return location;
	}
};

#endif // !SHADER_H
