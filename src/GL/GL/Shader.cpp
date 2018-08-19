/*
	Copyright (C) 2012 Alexander Overvoorde

	Permission is hereby granted, free of charge, to any person obtaining a copy of
	this software and associated documentation files (the "Software"), to deal in
	the Software without restriction, including without limitation the rights to
	use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
	the Software, and to permit persons to whom the Software is furnished to do so,
	subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
	FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
	COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
	IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
	CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE
*/

#include <GL/GL/Shader.hpp>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <logger.h>

namespace GL
{
	Shader::Shader(const Shader& rhs)
	{
		gc.Copy(m_ID, rhs.m_ID);
	}

	Shader::Shader(ShaderType::shader_type_t shader)
	{
		m_ID = gc.Create(glCreateShader(shader));
	}

	Shader::Shader(ShaderType::shader_type_t shader, const std::string& code)
	{
		m_ID = gc.Create(glCreateShader(shader));
		Source(code);
		Compile();
	}

	Shader::~Shader()
	{
		gc.Destroy(m_ID, m_DeleterFunc);
	}

	Shader::operator GLuint() const
	{
		return m_ID;
	}

	const Shader& Shader::operator=(const Shader& rhs)
	{
		gc.Destroy(m_ID, m_DeleterFunc);
		gc.Copy(m_ID, rhs.m_ID);
		return *this;
	}

	Shader Shader::LoadFromFile(ShaderType::shader_type_t  type, const std::string& filename)
	{
		GL::ID shader_id = -1;

		Z::INFO() << "Loading shader ... " << filename;

		// Read the file
		std::vector<char> contents;
		if (!getFileContents(filename, contents))
		{
			throw ShaderFileException();
		}

		//return compile(type , &shader[0] );
		return Shader(type, std::string(&contents[0]));

	}

	void Shader::Source(const std::string& code)
	{
		const char* c = code.c_str();
		glShaderSource(m_ID, 1, &c, NULL);
	}

	void Shader::Compile()
	{
		GLint res{ 0 };

		glCompileShader(m_ID);
		glGetShaderiv(m_ID, GL_COMPILE_STATUS, &res);

		if (res == GL_FALSE)
			throw CompileException(GetInfoLog());
	}

	std::string Shader::GetInfoLog()
	{
		GLint res{ 0 };
		glGetShaderiv(m_ID, GL_INFO_LOG_LENGTH, &res);

		if (res > 0)
		{
			std::string infoLog(res, 0);
			glGetShaderInfoLog(m_ID, res, &res, &infoLog[0]);
			return infoLog;
		}
		else {
			return "";
		}
	}

	GLuint Shader::Compile(GLenum type, const char* shaderCode) {

		GLuint _id = glCreateShader(type);
		glShaderSource(_id, 1, &shaderCode, NULL);
		glCompileShader(_id);

		GLint Result = false;
		int   InfoLogLength = 0;

		glGetShaderiv(_id, GL_COMPILE_STATUS, &Result);
		if (!Result) {
			Z::ERROR() << "Shader file compilation failed " << __FILE__ << __LINE__;
		}

		glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> ErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(_id, InfoLogLength, NULL, &ErrorMessage[0]);
			Z::ERROR() << string(&ErrorMessage[0]).c_str();
		}

		return _id;

	}
	// Read the contents of a file into an array of char
	bool Shader::getFileContents(const std::string& filename, std::vector<char>& buffer)
	{
		std::ifstream file(filename.c_str(), std::ios_base::binary);
		if (file)
		{
			file.seekg(0, std::ios_base::end);
			std::streamsize size = file.tellg();
			if (size > 0)
			{
				file.seekg(0, std::ios_base::beg);
				buffer.resize(static_cast<std::size_t>(size));
				file.read(&buffer[0], size);
			}
			buffer.push_back('\0');
			return true;
		}
		else
		{
			return false;
		}
	}

	GC Shader::gc;
}