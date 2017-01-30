#pragma once
#ifndef WINDOW_HPP
#define WINDOW_HPP


#include <Vulkan/vulkan.h>
#include "GLFW/glfw3.h"
#include <vector>
#include "RendererStructs.hpp"

class Window
{
public:
	Window();
	~Window();

	bool ShouldClose();

	VkSurfaceKHR GetWindowSurface();

	Swapchain GetSwapchain();

	void SetSwapchain(Swapchain chain);

	void CreateSurface(VkInstance instance);

	void CreateWindow();

	static void InitGLFW();

private:
	GLFWwindow* window;
	VkSurfaceKHR surface;

	Swapchain swapchain;	

	static bool initialized;
};

#endif // WINDOW_HPP
