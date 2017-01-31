#pragma once
#include <Vulkan\vulkan.h>
#include <vector>

struct Swapchain
{
	VkSwapchainKHR chain = VK_NULL_HANDLE;
	std::vector<VkImage> images();
	std::vector<VkImageView> views();

	VkSurfaceFormatKHR format{};
};

struct DeviceQueue
{
	int index = -1;
	VkQueue queue = VK_NULL_HANDLE;
};