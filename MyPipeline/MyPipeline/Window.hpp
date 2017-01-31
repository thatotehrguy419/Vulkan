#pragma once
#ifndef WINDOW_HPP
#define WINDOW_HPP


#include <Vulkan/vulkan.h>
#include <vector>
#include "RendererStructs.hpp"
#include "GLFW/glfw3.h"
#include "Renderer.hpp"

class Window
{
public:
	Window();
	~Window();

	bool ShouldClose();

	void CreateSwapchain();

	void CreateSurface();

	void CreateWindow();

	static void InitGLFW();

private:
	GLFWwindow* window = nullptr;
	VkSurfaceKHR surface = VK_NULL_HANDLE;

	Renderer renderer;

	Swapchain swapchain{};

	static bool initialized;
};

#endif // WINDOW_HPP
