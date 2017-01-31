#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "vulkan/vulkan.hpp"
#include "glfw/glfw3.h"
#include "Window.hpp"
#include <vector>
#include "RendererStructs.hpp"

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Init();

	void SetupPipeline();

	void Render();

	VkInstance GetInstance();

	VkPhysicalDevice GetPhysicalDevice();

	VkDevice GetDevice();

	DeviceQueue GetGraphicsQueue();
	DeviceQueue GetTransferQueue();
	DeviceQueue GetPresentQueue();

	void SetFormat(VkSurfaceFormatKHR format);

private:

	VkInstance instance = VK_NULL_HANDLE;

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice logicalDevice = VK_NULL_HANDLE;

	DeviceQueue graphicsQueue{};
	DeviceQueue presentQueue{};
	DeviceQueue transferQueue{};

	VkCommandPool cmdPool = VK_NULL_HANDLE;
	std::vector<VkCommandBuffer> cmdBuffers;

	VkRenderPass renderPass = VK_NULL_HANDLE;
	VkPipeline graphicsPipeline = VK_NULL_HANDLE;

	VkSurfaceFormatKHR presentationFormat{};

	void CreateInstance();

	void CreateDebugCallback();

	void PickPhysicalDevice();

	void CreateCommandBuffers();

	void CreateRenderPass();

	void CreatePipeline();

	void PrintLayers();

	void PrintExtensions();

	Renderer(Renderer &other);
	
};

#endif // !GRAPHICS_HPP