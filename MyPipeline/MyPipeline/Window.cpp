#include "Window.hpp"

extern bool const enableValidation;

Window::Window()
{
	CreateWindow();

	renderer.CreateInstance();

	if (enableValidation)
		renderer.CreateDebugCallback();

	renderer.CreateSurface(window);

	renderer.PickPhysicalDevice();

	renderer.CreateSwapchain();

	renderer.CreateCommandBuffers();

	renderer.CreateRenderPass();

	renderer.CreatePipeline();
}

Window::~Window()
{
	if (window != nullptr)
		glfwDestroyWindow(window);
}

void Window::Update()
{
	renderer.Render();
}

int Window::ShouldClose()
{
	return glfwWindowShouldClose(window);
}

void Window::CreateWindow()
{
	if (!initialized)
	{
		glfwInit();
		initialized = true;
	}

	if (window != nullptr)
	{
		glfwDestroyWindow(window);
		window = nullptr;
	}

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	window = glfwCreateWindow(800, 800, "Default Title", nullptr, nullptr);
}

bool Window::initialized(false);
