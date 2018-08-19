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

#include <GL/GL/Program.hpp>
#include <vector>

namespace GL
{
	Program::Program() {
		m_ID = m_GeneratorFunc();
		gc.Create(m_ID);
	}

	Program::Program(const Program& rhs) {
		gc.Copy(m_ID, rhs.m_ID);
	}

	Program::Program(const Shader& vertexShader) {
		m_ID = m_GeneratorFunc();
		gc.Create(m_ID);
		Attach(vertexShader);
		Link();
		glUseProgram(m_ID);
	}

	Program::Program(const Shader& vertex, const Shader& fragment)
	{
		m_ID = m_GeneratorFunc();
		gc.Create(m_ID);
		Attach(vertex);
		Attach(fragment);
		Link();
		glUseProgram(m_ID);
	}

	Program::Program(const Shader& vertex, const Shader& fragment, const Shader& geometry)
	{
		m_ID = m_GeneratorFunc();
		gc.Create(m_ID);
		Attach(vertex);
		Attach(fragment);
		Attach(geometry);
		Link();
		glUseProgram(m_ID);

	}

	const Program& Program::operator=(const Program& rhs) {
		gc.Destroy(m_ID, m_DeleterFunc);
		gc.Copy(m_ID, rhs.m_ID);
		return *this;
	}

	Program::~Program()
	{
		gc.Destroy(m_ID, m_DeleterFunc);
	}

	Program::operator GLuint() const
	{
		return m_ID;
	}

	void Program::Attach(const Shader& shader)
	{
		glAttachShader(m_ID, shader);
	}

	void Program::TransformFeedbackVaryings(const char** varyings, uint count)
	{
		glTransformFeedbackVaryings( m_ID ,  count, varyings, GL_INTERLEAVED_ATTRIBS);
	}

	void Program::Link()
	{
		GLint res;

		glLinkProgram(m_ID);
		glGetProgramiv(m_ID, GL_LINK_STATUS, &res);

		if (res == GL_FALSE)
			throw LinkException(GetInfoLog());
	}

	std::string Program::GetInfoLog()
	{
		GLint res;
		glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &res);

		if (res > 0)
		{
			std::string infoLog(res, 0);
			glGetProgramInfoLog(m_ID, res, &res, &infoLog[0]);
			return infoLog;
		}
		else {
			return "";
		}
	}

	Attribute Program::GetAttribute(const std::string& name)
	{
		return glGetAttribLocation(m_ID, name.c_str());
	}

	Uniform Program::GetUniform(const std::string& name)
	{
		return glGetUniformLocation(m_ID, name.c_str());
	}



	void Program::SetUniform(const Uniform& uniform, int value)
	{
		glUniform1i(uniform, value);
	}

	void Program::SetUniform(const Uniform& uniform, float value)
	{
		glUniform1f(uniform, value);
	}

	void Program::SetUniform(const Uniform& uniform, const Vec2& value)
	{
		glUniform2f(uniform, value.X, value.Y);
	}

	void Program::SetUniform(const Uniform& uniform, const Vec3& value)
	{
		glUniform3f(uniform, value.X, value.Y, value.Z);
	}

	void Program::SetUniform(const Uniform& uniform, const Vec4& value)
	{
		glUniform4f(uniform, value.X, value.Y, value.Z, value.W);
	}

	void Program::SetUniform(const Uniform& uniform, const float* values, GLuint count)
	{
		glUniform1fv(uniform, count, values);
	}

	void Program::SetUniform(const Uniform& uniform, const Vec2* values, GLuint count)
	{
		glUniform2fv(uniform, count, (float*)values);
	}

	void Program::SetUniform(const Uniform& uniform, const Vec3* values, GLuint count)
	{
		glUniform3fv(uniform, count, (float*)values);
	}

	void Program::SetUniform(const Uniform& uniform, const Vec4* values, GLuint count)
	{
		glUniform4fv(uniform, count, (float*)values);
	}

	void Program::SetUniform(const Uniform& uniform, const Mat3& value)
	{
		glUniformMatrix3fv(uniform, 1, GL_FALSE, value.m);
	}

	void Program::SetUniform(const Uniform& uniform, const Mat4& value)
	{
		glUniformMatrix4fv(uniform, 1, GL_FALSE, value.m);
	}

	GC Program::gc;
}