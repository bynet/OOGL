#pragma once
#include "assert.h"
#include <functional>
#include <GL/GL/GC.hpp>

class IndexBuffer {

public:
	GLuint m_ID{ 0 };

private:
	static GL::GC gc;
	std::function<void(GLsizei, GL::ID*)> m_GeneratorFunc{ glGenBuffers } ;
	std::function<void(GLsizei, const GL::ID*)> m_DeleterFunc{ glDeleteBuffers } ;

public:

	IndexBuffer();
	IndexBuffer(const GLvoid* data, GLsizei length, GLenum usage);
	IndexBuffer(const IndexBuffer& rhs) ;
	const IndexBuffer& operator=(const IndexBuffer& rhs) ;
	operator GLuint() const;
	~IndexBuffer();

	GL::ID id() const {
		return m_ID;
	}

	void Data(const GLvoid* data, GLsizeiptr lenght, GLenum usage);
	void SubData(const GLvoid* data, GLsizeiptr offset, GLsizeiptr length);
	void GetSubData(GLvoid* data, GLsizeiptr  offset, GLsizeiptr length);
	void Destroy();

};