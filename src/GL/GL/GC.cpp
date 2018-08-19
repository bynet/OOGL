#include <GL/GL/GC.hpp>

#if defined(OOGL_PLATFORM_WINDOWS) || defined( OOGL_PLATFORM_LINUX ) 
void GC::Create(GLuint& obj, createFunc c, deleteFunc d)
{
	c(1, &obj);
	refs.insert(std::pair<GLuint, uint>(obj, 1));

	this->d = d;
	this->d2 = 0;
}

int GC::Create(const GLuint& obj, deleteFunc2 d2)
{
	refs.insert(std::pair<GLuint, uint>(obj, 1));

	this->d = 0;
	this->d2 = d2;

	return obj;
}

void GC::Copy(const GLuint& from, GLuint& to, bool destructive = false)
{
	if (destructive)
		Destroy(to);

	to = from;
	refs[from]++;
}

void GC::Destroy(GLuint& obj)
{
	if (--refs[obj] == 0)
	{
		if (d != 0) d(1, &obj); else d2(obj);
		refs.erase(obj);
	}
}

#elif defined(OOGL_PLATFORM_SDL)

using namespace GL;
ID GC::Create(ID id) {
	refs.insert(std::pair<ID, uint16_t>(id, 1));
	return id;
}
ID GC::Create(function<ID()> generatorFunc) {
	ID obj{ 0 };
	obj = generatorFunc();
	refs.insert(std::pair<ID, uint16_t>(obj, 1));
	return obj;
}

ID GC::Create(function<void(GLsizei, GL::ID*)> generatorFunc) {
	ID obj = 0;
	generatorFunc(1, &obj);
	refs.insert(std::pair<ID, uint16_t>(obj, 1));
	return obj;
}

void GC::Copy(ID& dst, const ID& src) {
	dst = src;
	refs[dst]++;
}
void GC::Destroy(ID& obj, std::function<void(GLsizei, const ID*)> deleterFunc) {
	if (--refs[obj] == 0) {
		deleterFunc(1, &obj);
		refs.erase(obj);
		obj = 0;
	}
}

void GC::Destroy(ID& obj, std::function<void(GLuint)> deleterFunc) {
	if (--refs[obj] == 0) {
		deleterFunc(obj);
		refs.erase(obj);
		obj = 0;
	}
}

#endif


