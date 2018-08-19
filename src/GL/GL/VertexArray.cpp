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

#include <GL/GL/VertexArray.hpp>

namespace GL
{
	VertexArray::VertexArray()
	{
		m_ID = gc.Create(m_GeneratorFunc);
	}

	VertexArray::VertexArray(const VertexArray& rhs)
	{
		gc.Copy(m_ID, rhs.m_ID);
	}

	VertexArray::~VertexArray()
	{
		gc.Destroy(m_ID, m_DeleterFunc);
	}



	VertexArray::operator GLuint() const
	{
		return m_ID;
	}

	const VertexArray& VertexArray::operator=(const VertexArray& rhs)
	{
		gc.Destroy(m_ID, m_DeleterFunc);
		gc.Copy(m_ID, rhs.m_ID);
		return *this;
	}


	void VertexArray::BindAttribute(const Attribute& attribute, const VertexBuffer& buffer, Type::type_t type, uint count, uint stride, intptr_t offset)
	{
		glBindVertexArray(m_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer); //check this : cihangir
		glEnableVertexAttribArray(attribute);
		glVertexAttribPointer(attribute, count, type, GL_FALSE, stride, (const GLvoid*)offset);
	}

	void VertexArray::BindElements(const VertexBuffer& elements)
	{
		glBindVertexArray(m_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elements);
	}

	void VertexArray::BindTransformFeedback(uint index, const VertexBuffer& buffer)
	{
		glBindVertexArray(m_ID);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, index, buffer);
	}


	GC VertexArray::gc;
}