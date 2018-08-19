/*
Copyright (C) 2018 Cihangir Zengin

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

#include <GL/Window/Window.hpp>
#include <Utility/logger.h>

#ifdef OOGL_PLATFORM_SDL

#include <SDL.h>
#include <GL/Window/Event.hpp>
#include <assert.h>
#include <GL/GL/Context.hpp>
#include <GL/GL/Error.hpp>

namespace GL
{
	Window::Window(uint width, uint height, const std::string& title, WindowStyle::window_style_t style)
	{

		if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0) {
			Z::ERROR() << "Failed to init SDL ";
			throw WindowException();
		}

		// Configure window style
		uint32_t windowStyle = SDL_WINDOW_OPENGL;
		if (style & WindowStyle::Resize) windowStyle |= SDL_WINDOW_RESIZABLE;

		m_WindowPtr = SDL_CreateWindow(title.c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			width,
			height,
			windowStyle);

		if (m_WindowPtr == nullptr) {

			Z::ERROR() << "Unable to create SDL2 window! " << SDL_GetError();
			throw WindowException();
		}
		m_Context = SDL_GL_CreateContext(m_WindowPtr);
		if (m_Context == 0) {
			throw(GL::Exception("Failed to create opengl context"));
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		// OpenGL verison 4.5 (released 2014)
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

		// Enable double buffering
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		//Use Vsync
		SDL_GL_SetSwapInterval(1);

		// init glew 
		glewExperimental = GL_TRUE;
		GLenum glewError = glewInit();
		assert(glewError == GLEW_OK && glewGetErrorString(glewError));

		// Clear framebuffer with black
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		glEnable(GL_DEPTH_TEST);

		// transparency
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Check for errors.
		//glGetLastError();
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallback, 0);

		int x = 0;
		int y = 0;

		if (style & WindowStyle::Fullscreen) {
			windowStyle |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		int w = 0;
		int h = 0;
		SDL_GetWindowSize(m_WindowPtr, &w, &h);
		width = (uint)w;
		height = (uint)h;
		SDL_GetWindowPosition(m_WindowPtr, &x, &y);


		this->open = true;
		this->style = windowStyle;
		this->mousex = 0;
		this->mousey = 0;
		memset(this->mouse, 0, sizeof(this->mouse));
		memset(this->keys, 0, sizeof(this->keys));
		this->context = nullptr;
	}

	Window::~Window()
	{
		if (m_WindowPtr != nullptr) {
			SDL_GL_DeleteContext(m_Context);
			SDL_DestroyWindow(m_WindowPtr);
			SDL_Quit();
		}
	}

	void Window::SetPos(int x, int y)
	{
		if (!open) return;
		SDL_SetWindowPosition(m_WindowPtr, x, y);
	}

	void Window::SetSize(uint width, uint height)
	{
		if (!open) return;
		SDL_SetWindowSize(m_WindowPtr, width, height);
	}

	void Window::SetTitle(const std::string& title)
	{
		if (!open) return;
		SDL_SetWindowTitle(m_WindowPtr, title.c_str());
	}

	void Window::SetVisible(bool visible)
	{
		if (!open) return;
		if (visible) {
			SDL_HideWindow(m_WindowPtr);
		}
		else {
			SDL_ShowWindow(m_WindowPtr);
		}
	}

	void Window::Close()
	{
		SDL_DestroyWindow(m_WindowPtr);
		m_WindowPtr = nullptr;
		open = false;
	}

	bool Window::GetEvent(Event& ev)
	{
		// Fetch new events
		WindowEvent();

		// Return oldest event - if available
		if (events.empty()) return false;

		ev = events.front();
		events.pop();

		return true;
	}

	void Window::Present()
	{
		SwapBuffers();
	}

	void Window::SwapBuffers() const {
		assert(m_WindowPtr != nullptr);
		SDL_GL_SwapWindow(m_WindowPtr);
	}

	void Window::WindowEvent()
	{
		Event ev;

		SDL_Event event;
		while (SDL_PollEvent(&event)) {

			ev.Type = Event::Unknown;

			switch (event.type)
			{
			case SDL_WINDOWEVENT:
			{
				switch (event.window.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
					if (events.empty()) {
						ev.Type = Event::Resize;
						ev.Window.Width = event.window.data1;
						ev.Window.Height = event.window.data2;
					}
					else if (events.back().Type == Event::Resize) {
						events.back().Window.Width = event.window.data1;
						events.back().Window.Height = event.window.data2;
					}
					break;
				case SDL_WINDOWEVENT_MOVED:
					if (events.empty()) {
						ev.Type = Event::Move;
						ev.Window.X = x;
						ev.Window.Y = y;
					}
					else if (events.back().Type == Event::Move) {
						events.back().Window.X = x;
						events.back().Window.Y = y;
					}

					break;
				case SDL_WINDOWEVENT_FOCUS_GAINED:
					ev.Type = Event::Focus;
					focus = true;
					break;
				case SDL_WINDOWEVENT_FOCUS_LOST:
					ev.Type = Event::Blur;
					focus = false;
					break;
				case SDL_WINDOWEVENT_CLOSE:
					ev.Type = Event::Close;
					break;
				}

				break;
			}
			case SDL_KEYDOWN:
				ev.Type = Event::KeyDown;
				ev.Key.Code = TranslateKey(event.key.keysym.sym);
				ev.Key.Alt = (event.key.keysym.mod & KMOD_ALT) ? true : false;
				ev.Key.Control = (event.key.keysym.mod & KMOD_CTRL) ? true : false;
				ev.Key.Shift = (event.key.keysym.mod & KMOD_SHIFT) ? true : false;
				keys[ev.Key.Code] = true;
				break;

			case SDL_KEYUP:
				ev.Type = Event::KeyUp;
				ev.Type = Event::KeyDown;
				ev.Key.Code = TranslateKey(event.key.keysym.sym);
				ev.Key.Alt = (event.key.keysym.mod & KMOD_ALT) ? true : false;
				ev.Key.Control = (event.key.keysym.mod & KMOD_CTRL) ? true : false;
				ev.Key.Shift = (event.key.keysym.mod & KMOD_SHIFT) ? true : false;
				keys[ev.Key.Code] = true;
				break;

			case SDL_QUIT:
				ev.Type = Event::Close;
				break;

			}

			if (ev.Type != Event::Unknown) {
				events.push(ev);
			}
		}

	}


	Key::key_t Window::TranslateKey(SDL_Keycode code)
	{
		switch (code)
		{
		case SDLK_RSHIFT:
		case SDLK_LSHIFT:
			return Key::Shift;

		case SDLK_MENU:
			return Key::Alt;

		case SDLK_RCTRL:
		case SDLK_LCTRL:
			return Key::Control;

		case SDLK_SEMICOLON:
			return Key::Semicolon;

		case SDLK_SLASH:
			return Key::Slash;

		case SDLK_EQUALS:
			return Key::Equals;

		case SDLK_MINUS:
			return Key::Hyphen;

		case SDLK_LEFTBRACKET:
			return Key::LeftBracket;

		case SDLK_RIGHTBRACKET:
			return Key::RightBracket;

		case SDLK_COMMA:
			return Key::Comma;

		case SDLK_PERIOD:
			return Key::Period;

		case SDLK_QUOTE:
			return Key::Quote;

		case SDLK_BACKSLASH:
			return Key::Backslash;

		case SDLK_SPACE:
			return Key::Space;

		case SDLK_RETURN:
			return Key::Enter;

		case SDLK_BACKSPACE:
			return Key::Backspace;

		case SDLK_TAB:
			return Key::Tab;

		case SDLK_PAGEUP:
			return Key::PageUp;

		case SDLK_PAGEDOWN:
			return Key::PageDown;

		case SDLK_HOME:
			return Key::Home;

		case SDLK_END:
			return Key::End;

		case SDLK_INSERT:
			return Key::Insert;

		case SDLK_DELETE:
			return Key::Delete;

		case SDLK_PLUS:
			return Key::Add;

		case SDLK_PAUSE:
			return Key::Pause;

		case SDLK_LEFT:
			return Key::Left;

		case SDLK_RIGHT:
			return Key::Right;

		case SDLK_UP:
			return Key::Up;

		case SDLK_DOWN:
			return Key::Down;
		}


		return Key::Unknown;
	}
	bool Window::PollEvents() {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type)
			{
			case SDL_WINDOWEVENT:
			{
				switch (event.window.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
					//onResize(event.window.data1, event.window.data2);
					break;
				}

				break;
			}

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					return false;
				}
				break;
			case SDL_QUIT:
				return false;

			}
		}
		return true;
	}

	Context& Window::GetContext()
	{
		if (context)
			return *context;
		else
			return *(context = new Context());
	}

}
#endif