#include "DebugMode.hpp"
#include "vulkan/vulkan.h"
#include "GLFW/glfw3.h"
#include "Window.hpp"

void main()
{
	Window window;

	while (!window.ShouldClose())
	{
		glfwPollEvents();
		window.Update();
	}
}
