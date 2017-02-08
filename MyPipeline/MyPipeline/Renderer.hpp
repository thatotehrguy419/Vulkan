#pragma once
#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <iostream>
#include <vector>
#include "vulkan/vulkan.hpp"
#include "GLFW/glfw3.h"
#include "RendererUtils.hpp"

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Init();

	void Render();

	void CreateInstance();

	void CreateDebugCallback();

	void CreateSurface(GLFWwindow* window);

	void PickPhysicalDevice();

	void CreateCommandBuffers();

	void CreateSwapchain();

	void CreateRenderPass();

	void CreatePipeline();

private:

	VkDebugReportCallbackEXT debugCallback = VK_NULL_HANDLE;
	PFN_vkCreateDebugReportCallbackEXT CreateCallback = nullptr;
	PFN_vkDestroyDebugReportCallbackEXT DestroyCallback = nullptr;

	VkInstance instance = VK_NULL_HANDLE;

	Swapchain swapchain;
	VkSurfaceKHR surface = VK_NULL_HANDLE;

	PhysicalDevice physicalDevice;
	VkDevice logicalDevice = VK_NULL_HANDLE;

	DeviceQueue graphicsQueue{};
	DeviceQueue presentQueue{};
	DeviceQueue transferQueue{};

	VkCommandPool graphicsCmdPool = VK_NULL_HANDLE;
	VkCommandPool transferCmdPool = VK_NULL_HANDLE;
	std::vector<VkCommandBuffer> graphicsCmdBuffers;
	std::vector<VkCommandBuffer> transferCmdBuffers;

	ImageBuffer depthImage{};
	Buffer vertexStagingBuffer{};
	Buffer vertexBuffer{};
	Buffer uniformBuffer{};

	VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
	VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
	VkDescriptorSetLayout descriptorLayout = VK_NULL_HANDLE;

	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline = VK_NULL_HANDLE;
	
	VkRenderPass renderPass = VK_NULL_HANDLE;
	std::vector<VkFramebuffer> frameBuffers;

	VkShaderModule vertexModule = VK_NULL_HANDLE;
	VkShaderModule fragmentModule = VK_NULL_HANDLE;

	VkSemaphore imageacquired = VK_NULL_HANDLE;
	VkSemaphore renderDone = VK_NULL_HANDLE;



	void PrintLayers();

	void PrintExtensions();

	void CreatePipelineBuffers();

	void CreateDepthImage();

	void CreateDescriptors();

	void CreateBuffer(Buffer* buffer, VkDeviceSize size, VkBufferUsageFlags usage, std::set<uint32_t> indices);

	void CreateShaderModule(VkShaderModule* module, const char* file);

	void DestroyInstance();

	void DestroyValidationCallback();

	void DestroyDevice();

	void DestroySurface();

	void DestroySwapChain();

	void DestroyCommandPools();

	void DestroyDescriptors();

	void DestroyDepthImage();

	void DestroyRenderPass();

	void DestroyBuffers();

	void DestroyShaderModules();

	void DestroyPipeline();


	Renderer(Renderer &other);
	
};

#endif // RENDERER_HPP