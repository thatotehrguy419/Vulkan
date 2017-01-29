#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "vulkan/vulkan.hpp"
#include "glfw/glfw3.h"
#include "Window.hpp"
#include <vector>

struct DeviceQueue 
{
	int index = -1;
	VkQueue queue = VK_NULL_HANDLE;
};

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Init(Window& window);

	void Render();

private:

	VkInstance instance;

	VkPhysicalDevice physicalDevice;
	VkDevice logicalDevice;

	DeviceQueue graphicsQueue;
	DeviceQueue presentQueue;
	DeviceQueue transferQueue;

	VkCommandPool cmdPool;
	std::vector<VkCommandBuffer> cmdBuffers;

	VkRenderPass renderPass;
	VkPipeline graphicsPipeline;

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