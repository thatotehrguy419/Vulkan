#include "vulkan/vulkan.h"
#include "GLFW/glfw3.h"
#include "Renderer.hpp"
#include "Window.hpp"

void main()
{
	glfwInit();

	Window window;

	Renderer renderer;

	renderer.Init();


	while (!window.ShouldClose())
	{
		glfwPollEvents();
	}
}
