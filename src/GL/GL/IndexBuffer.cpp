
#include <GL/GL/IndexBuffer.hpp>

GL::GC IndexBuffer::gc;

IndexBuffer::operator GLuint() const {
	return m_ID;
}

void IndexBuffer::Data(const GLvoid* data, GLsizeiptr  lenght, GLenum usage) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, lenght, data, usage);
}
				
void IndexBuffer::SubData(const GLvoid* data, GLsizeiptr offset , GLsizeiptr length){
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID );
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, length, data);
}

void IndexBuffer::GetSubData(GLvoid* data, GLsizeiptr  offset, GLsizeiptr length){
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID );
	glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, length, data);
}

IndexBuffer::IndexBuffer() {
	m_ID = gc.Create(m_GeneratorFunc);
}

IndexBuffer::IndexBuffer(const IndexBuffer& rhs) {
	gc.Copy(m_ID, rhs.m_ID);
}

const IndexBuffer& IndexBuffer::operator=(const IndexBuffer& rhs) {
//	gc.Destroy(m_ID, m_DeleterFunc);
	gc.Copy(m_ID, rhs.m_ID);
	return *this;
}

IndexBuffer::~IndexBuffer() {
	gc.Destroy(m_ID, m_DeleterFunc);
}

IndexBuffer::IndexBuffer(const GLvoid* data, GLsizei length, GLenum usage) {
	m_ID = gc.Create(m_GeneratorFunc);
	Data(data, length, usage);
}

void IndexBuffer::Destroy() {
	gc.Destroy(m_ID, m_DeleterFunc);
}
