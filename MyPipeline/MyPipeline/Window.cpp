#include "Window.hpp"

Window::Window()
{
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

Swapchain Window::GetSwapchain()
{
	return swapchain;
}

void Window::SetSwapchain(Swapchain chain)
{
	swapchain = chain;
}

void Window::CreateSurface(VkInstance instance)
{
	if(surface != VK_NULL_HANDLE)
		vkDestroySurfaceKHR()
	glfwCreateWindowSurface(instance, window, nullptr, &surface);
}

void Window::CreateWindow()
{
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	window = glfwCreateWindow(800, 800, "Default Title", nullptr, nullptr);
}

void Window::InitGLFW()
{
	if (!initialized)
	{
		glfwInit();
		initialized = true;
	}
}

bool Window::initialized(false);
