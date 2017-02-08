#pragma once
#ifndef RENDERER_UTILS_HPP
#define RENDERER_UTILS_HPP

#include <Vulkan\vulkan.h>
#include <vector>
#include <set>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>




////////////////////////////////////////////////////////////////////////////////////////////
////									STRUCTS											////
////////////////////////////////////////////////////////////////////////////////////////////

struct Swapchain
{
	VkSwapchainKHR chain = VK_NULL_HANDLE;
	std::vector<VkImage> images;
	std::vector<VkImageView> views;

	VkSurfaceFormatKHR surfaceFormat{};
	VkExtent2D extent;
};

struct DeviceQueue
{
	int index = -1;
	VkQueue queue = VK_NULL_HANDLE;
};

struct Buffer
{
	VkBuffer buffer = VK_NULL_HANDLE;
	VkDeviceMemory memory = VK_NULL_HANDLE;
	VkDeviceSize memorysize;
};

struct ImageBuffer
{
	VkImage image = VK_NULL_HANDLE;
	VkImageView view = VK_NULL_HANDLE;
	VkDeviceMemory memory = VK_NULL_HANDLE;
};

struct PhysicalDevice
{
	VkPhysicalDevice device = VK_NULL_HANDLE;
	VkPhysicalDeviceFeatures features{};
	VkPhysicalDeviceMemoryProperties memProperties{};
};

struct Vertex
{
	glm::vec4 position;
	glm::vec4 color;
};

////////////////////////////////////////////////////////////////////////////////////////////
////									FUNCTIONS										////
////////////////////////////////////////////////////////////////////////////////////////////


uint32_t FindMemoryTypeIndex(PhysicalDevice physicalDevice, uint32_t typebits, VkMemoryPropertyFlags properties);

std::vector<char> LoadShaderCode(const char* file);

#endif