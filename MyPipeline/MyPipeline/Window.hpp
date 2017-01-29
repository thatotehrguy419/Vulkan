#pragma once
#ifndef WINDOW_HPP
#define WINDOW_HPP


#include <Vulkan/vulkan.h>
#include "GLFW/glfw3.h"
#include <vector>

class Window
{
public:
	Window();
	~Window();

	bool ShouldClose();

	VkSurfaceKHR GetWindowSurface();

	VkSwapchainKHR GetSwapchain();
	VkImage GetChainImage(int index);
	VkImageView GetChainView(int index);
	VkFormat GetChainFormat();

	static void InitGLFW();

private:
	static bool initialized;

	GLFWwindow* window;
	VkSurfaceKHR surface;

	VkSwapchainKHR swapchain;
	VkFormat chainFormat;
	std::vector<VkImage> chainImages;
	std::vector<VkImageView> chainViews;

	
};

#endif // WINDOW_HPP
