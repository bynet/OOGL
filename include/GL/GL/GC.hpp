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

#pragma once

#ifndef OOGL_GC_HPP
#define OOGL_GC_HPP

#include <GL/Platform.hpp>
#include <functional>

#if defined( OOGL_PLATFORM_WINDOWS) || defined(OOGL_PLATFORM_UNIX)
 #include <map>
#elif defined(OOGL_PLATFORM_SDL)
 #include <unordered_map>
#endif

using namespace std;
namespace GL
{
	/*
		OpenGL object creation/destruction function prototypes
	*/
#if defined( OOGL_PLATFORM_WINDOWS) 
	typedef void(__stdcall * createFunc) (GLsizei, GLuint*);
	typedef void(__stdcall * deleteFunc) (GLsizei, const GLuint*);
	typedef void(__stdcall * deleteFunc2) (GLuint);
#elif defined( OOGL_PLATFORM_LINUX )
	typedef void(*createFunc) (GLsizei, GLuint*);
	typedef void(*deleteFunc) (GLsizei, const GLuint*);
	typedef void(*deleteFunc2) (GLuint);
#endif

	/*
		OpenGL object garbage collector
	*/
	class GC
	{
#if defined(OOGL_PLATFORM_WINDOWS) || defined( OOGL_PLATFORM_LINUX ) 
	public:

		void Create(GLuint& obj, createFunc c, deleteFunc d);
		int  Create(const GLuint& obj, deleteFunc2 d2);
		void Copy(const GLuint& from, GLuint& to, bool destructive = false);
		void Destroy(GLuint& obj);

	private:
		std::map<GLuint, uint> refs;
		deleteFunc d;
		deleteFunc2 d2;

#elif defined(OOGL_PLATFORM_SDL)
	private:

		unordered_map<ID, uint16_t> refs;

	public:

		ID   Create(ID id);
		ID   Create(function<ID()>);
		ID   Create(function<void(GLsizei, GL::ID*)> generatorFunc);

		void Copy(ID& dst, const ID& src);
		void Destroy(ID& obj, std::function<void(GLsizei, const ID*)> deleterFunc);
		void Destroy(ID& obj, std::function<void(GLuint)> deleterFunc);

#endif
	};
};

#endif