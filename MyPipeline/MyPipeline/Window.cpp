#include "Window.hpp"

Window::Window()
{

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(800, 800, "Default Title", nullptr, nullptr);

}

Window::~Window()
{
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(window);
}

VkSurfaceKHR Window::GetWindowSurface()
{
	return surface;
}

VkSwapchainKHR Window::GetSwapchain()
{
	return swapchain;
}

VkImage Window::GetChainImage(int index)
{
	return chainImages[index];
}

VkImageView Window::GetChainView(int index)
{
	return chainViews[index];
}

VkFormat Window::GetChainFormat()
{
	return chainFormat;
}

void Window::InitGLFW()
{
	if (!initialized)
		glfwInit();
}

bool Window::initialized(false);
