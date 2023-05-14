/*****************************************************************
*\file		 Window.cpp
*\brief		 Window functions definitions.

*\author(s)   Lee Jun Jie	144 lines x % Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <Window.h>
#include <FakeEngine.h>
extern std::unique_ptr<FakeEngine> gFakeEngine;

bool Window::Init(const int width, const int height, std::string  title)
{
	_width = width;
	_height = height;
	_title = title;

	if (!glfwInit()) {
		std::cout << "GLFW init has failed - abort program!!!" << std::endl;
		return false;
	}

	// In case a GLFW function fails, an error is reported to callback function
	glfwSetErrorCallback(Window::error_cb);
	// in that context:
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_RED_BITS, 8); glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8); glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // window dimensions are static

	Window::ptr_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	ImGui_ImplGlfwGL3_Init(Window::ptr_window, true);

	if (!Window::ptr_window) {
		std::cerr << "GLFW unable to create OpenGL context - abort program\n";
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(Window::ptr_window);
	glfwSetFramebufferSizeCallback(Window::ptr_window, Window::fbsize_cb);
	glfwSetKeyCallback(Window::ptr_window, InputCallBack::key_cb);
	glfwSetMouseButtonCallback(Window::ptr_window, InputCallBack::mousebutton_cb);
	glfwSetCursorPosCallback(Window::ptr_window, InputCallBack::mousepos_cb);
	glfwSetScrollCallback(Window::ptr_window, InputCallBack::mousescroll_cb);
	glfwSetInputMode(Window::ptr_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetWindowFocusCallback(Window::ptr_window, Window::window_focus_cb);

	// Initialize entry points to OpenGL functions and extensions
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Unable to initialize GLEW - error: "
			<< glewGetErrorString(err) << " abort program" << std::endl;
		return false;
	}
	if (GLEW_VERSION_4_5) {
		std::cout << "Using glew version: " << glewGetString(GLEW_VERSION) << std::endl;
		std::cout << "Driver supports OpenGL 4.5\n" << std::endl;
	}
	else if (GLEW_VERSION_4_3) {
		std::cout << "Using glew version: " << glewGetString(GLEW_VERSION) << std::endl;
		std::cout << "Driver supports OpenGL 4.3\n" << std::endl;
	}
	else {
		std::cerr << "Driver doesn't support OpenGL 4.5 - abort program" << std::endl;
		return false;
	}
	Window::print_specs();
	return true;
}

void Window::cleanup() {
	glfwTerminate();
}

void Window::error_cb(int error, char const* description) {
	std::cerr << "GLFW error: " << description << std::endl;
	(void)error;
}

void Window::fbsize_cb(GLFWwindow* ptr_win, int width, int height) {
	std::cout << "fbsize_cb getting called!!!" << std::endl;
	glViewport(0, 0, width, height);
	Window& window = gFakeEngine->GetGraphicManager<Window>();
	window._width = width;
	window._height = height;
	(void)ptr_win;
}

void Window::print_specs()
{
	// information is printed
	std::cout << "----------------------------------------------------------------------\n " << std::endl;
	GLubyte const* str_ven = glGetString(GL_VENDOR);
	std::cout << "GPU Vendor: " << str_ven << std::endl;
	str_ven = glGetString(GL_RENDERER);
	std::cout << "GL Renderer: " << str_ven << std::endl;
	str_ven = glGetString(GL_VERSION);
	std::cout << "GL Version: " << str_ven << std::endl;
	str_ven = glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::cout << "GL Shader Version: " << str_ven << std::endl;
	GLint major;
	GLint minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	std::cout << "GL Major Version: " << major << std::endl;
	std::cout << "GL Minor Version: " << minor << std::endl;
	GLint buffer;
	glGetFramebufferParameteriv(GL_FRAMEBUFFER, GL_DOUBLEBUFFER, &buffer);
	if (buffer)
		std::cout << "Current OpenGL Context is double buffered" << std::endl;
	GLint maxVertices;
	glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &maxVertices);
	std::cout << "Maximum Vertex Count: " << maxVertices << std::endl;
	GLint maxIndices;
	glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &maxIndices);
	std::cout << "Maximum Indices Count: " << maxIndices << std::endl;
	GLint tex_size;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &tex_size);
	std::cout << "Maximum texture size: " << tex_size << std::endl;
	GLint m_viewport[2];
	glGetIntegerv(GL_MAX_VIEWPORT_DIMS, m_viewport);
	std::cout << "Maximum Viewport Dimensions: " << m_viewport[0] << " x " << m_viewport[1] << std::endl;
	std::cout << "----------------------------------------------------------------------\n" << std::endl;
}

void Window::PrintWindowTitle()
{
	std::stringstream sstr;
	sstr << std::fixed
		<< std::setprecision(2)
		<< _title
		<< " | Level 1"
		<< " | FPS: "
		<< gFakeEngine->GetGraphicManager<FPS>().fps;
	glfwSetWindowTitle(ptr_window, sstr.str().c_str());
}

void Window::SetWindowResolution(int width, int height)
{
	_width = width;
	_height = height;
	glfwSetWindowSize(ptr_window, width, height);
}

void Window::SetWindowFullScreen(bool check)
{
	_fullscreen = check;
	glfwSetWindowMonitor(Window::ptr_window, _fullscreen ? glfwGetPrimaryMonitor() : NULL, 0, 0, _width, _height, GLFW_DONT_CARE);
	if(!_fullscreen)
		glfwSetWindowPos(Window::ptr_window, 0, 25);
}

void Window::window_focus_cb(GLFWwindow* ptr_win, int focused)
{
	(void)ptr_win;
	Window& _window = gFakeEngine->GetGraphicManager<Window>();
	if (focused)
		_window._focus = true;
	else
		_window._focus = false;
}

void Window::hideMouseCursor(bool check)
{
	(check) ? glfwSetInputMode(Window::ptr_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN) : glfwSetInputMode(Window::ptr_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}