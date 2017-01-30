#include "vulkan/vulkan.h"
#include "GLFW/glfw3.h"
#include "Renderer.hpp"
#include "Window.hpp"

void main()
{
	Window::InitGLFW();
	Window window;
	window.CreateWindow();

	Renderer renderer;
	renderer.Init(window);


	while (!window.ShouldClose())
	{
		glfwPollEvents();
	}
}
