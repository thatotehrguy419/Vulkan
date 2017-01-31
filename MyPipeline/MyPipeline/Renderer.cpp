//#define NDEBUG
#include "Renderer.hpp"
#include <iostream>
#include <set>
#include <algorithm>

#ifdef NDEBUG
const bool enableValidation = false;
#else
const bool enableValidation = true;

VkDebugReportCallbackEXT debugCallback;
PFN_vkCreateDebugReportCallbackEXT CreateCallback;
PFN_vkDestroyDebugReportCallbackEXT DestroyCallback;

VKAPI_ATTR VkBool32 VKAPI_CALL PrintMessage(
	VkDebugReportFlagsEXT                       flags,
	VkDebugReportObjectTypeEXT                  objectType,
	uint64_t                                    object,
	size_t                                      location,
	int32_t                                     messageCode,
	const char*                                 pLayerPrefix,
	const char*                                 pMessage,
	void*                                       pUserData)
{
	std::cerr << "Standard Validation: " << pMessage << std::endl;

	return VK_TRUE;
}
#endif

#include <cassert>

Renderer::Renderer()
{

}

Renderer::Renderer(Renderer &other)
{

}

Renderer::~Renderer()
{

	if (logicalDevice != VK_NULL_HANDLE)
		vkDestroyDevice(logicalDevice, nullptr);

	if (debugCallback != VK_NULL_HANDLE)
		DestroyCallback(instance, debugCallback, nullptr);

	if (instance != VK_NULL_HANDLE)
		vkDestroyInstance(instance, nullptr);
	
}

void Renderer::Render()
{

}

VkInstance Renderer::GetInstance()
{
	return instance;
}

VkPhysicalDevice Renderer::GetPhysicalDevice()
{
	return physicalDevice;
}

VkDevice Renderer::GetDevice()
{
	return logicalDevice;
}

DeviceQueue Renderer::GetGraphicsQueue()
{
	return graphicsQueue;
}

DeviceQueue Renderer::GetTransferQueue()
{
	return transferQueue;
}

DeviceQueue Renderer::GetPresentQueue()
{
	return presentQueue;
}

void Renderer::SetFormat(VkSurfaceFormatKHR format)
{
	presentationFormat = format;
}

void Renderer::Init()
{
	CreateInstance();

	if (enableValidation)
		CreateDebugCallback();

	PickPhysicalDevice();
}

void Renderer::SetupPipeline()
{
}

void Renderer::CreateInstance()
{
	VkResult result;
	std::vector<const char*> layers;
	std::vector<const char*> extensions;
	VkApplicationInfo appinfo{};
	VkInstanceCreateInfo info{};
	uint32_t count;
	const char** windowExts;

	layers.push_back("VK_LAYER_LUNARG_standard_validation");
	windowExts = glfwGetRequiredInstanceExtensions(&count);
	for (uint32_t i = 0; i < count; i++)
		extensions.push_back(windowExts[i]);

	appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appinfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appinfo.pApplicationName = "MyPipeline";
	appinfo.apiVersion = VK_MAKE_VERSION(1, 0, 37);

	info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	info.pApplicationInfo = &appinfo;
	if (enableValidation)
	{
		PrintLayers();
		PrintExtensions();
		info.ppEnabledLayerNames = layers.data();
		info.enabledLayerCount = layers.size();
		extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}
	info.ppEnabledExtensionNames = extensions.data();
	info.enabledExtensionCount = extensions.size();

	result = vkCreateInstance(&info, nullptr, &instance);
	assert(result == VK_SUCCESS);

}

void Renderer::CreateDebugCallback()
{	
	VkResult result;

	CreateCallback = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
	DestroyCallback = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");

	VkDebugReportCallbackCreateInfoEXT info{};
	info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
	info.pfnCallback = (PFN_vkDebugReportCallbackEXT)PrintMessage;

	result = CreateCallback(instance, &info, nullptr, &debugCallback);

	assert(result == VK_SUCCESS);
}

void Renderer::PickPhysicalDevice()
{
	VkResult result;

	uint32_t count;

	std::vector<VkPhysicalDevice> devices;
	vkEnumeratePhysicalDevices(instance, &count, nullptr);
	devices.resize(count);
	vkEnumeratePhysicalDevices(instance, &count, devices.data());

	count = 0;
	for (VkPhysicalDevice d : devices)
	{
		count++;
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(d, &properties);

		std::vector<VkQueueFamilyProperties> qfamilies;
		vkGetPhysicalDeviceQueueFamilyProperties(d, &count, nullptr);
		qfamilies.resize(count);
		vkGetPhysicalDeviceQueueFamilyProperties(d, &count, qfamilies.data());

		for (uint32_t i = 0; i < count; i++)
		{
			if (qfamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
				graphicsQueue.index = i;

			VkBool32 supportSurface;

			if (supportSurface)
				presentQueue.index = i;

			if (qfamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT && i != graphicsQueue.index)
				transferQueue.index = i;

			if (graphicsQueue.index > -1 && presentQueue.index > -1 && transferQueue.index > -1)
			{
				physicalDevice = d;
				break;
			}
		}

		if (graphicsQueue.index > -1 && transferQueue.index == -1 && qfamilies[graphicsQueue.index].queueFlags & VK_QUEUE_TRANSFER_BIT)
		{
			transferQueue.index = graphicsQueue.index;
			physicalDevice = d;
		}

		assert(physicalDevice != VK_NULL_HANDLE);

		if (enableValidation)
		{
			const char* separator = "#########################";
			std::cout << separator << std::endl << "Device " << count << std::endl;
			vkEnumerateDeviceExtensionProperties(d, nullptr, &count, nullptr);
			std::vector<VkExtensionProperties> extprops(count);
			vkEnumerateDeviceExtensionProperties(d, nullptr, &count, extprops.data());

			std::cout << "\t Name: " << properties.deviceName << std::endl;
			for (VkExtensionProperties p : extprops)
			{
				std::cout << "\t " << p.extensionName << std::endl;
			}

			std::cout << std::endl;
		}

	}

	std::set<int> families = { graphicsQueue.index, presentQueue.index, transferQueue.index};

	std::vector<VkDeviceQueueCreateInfo> qinfos;
	for (int f : families)
	{
		float* priorities = new float[1]{ 1.0f };
		VkDeviceQueueCreateInfo qinfo{};
		qinfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		qinfo.queueCount = 1;
		qinfo.queueFamilyIndex = f;
		qinfo.pQueuePriorities = priorities;

		qinfos.push_back(qinfo);
	}

	char** extensions = new char*[1]{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	VkDeviceCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	info.ppEnabledExtensionNames = extensions;
	info.enabledExtensionCount = 1;
	info.pQueueCreateInfos = qinfos.data();
	info.queueCreateInfoCount = qinfos.size();

	result = vkCreateDevice(physicalDevice, &info, nullptr, &logicalDevice);
	assert(result == VK_SUCCESS);


	vkGetDeviceQueue(logicalDevice, graphicsQueue.index, 0, &graphicsQueue.queue);
	vkGetDeviceQueue(logicalDevice, presentQueue.index, 0, &presentQueue.queue);
	vkGetDeviceQueue(logicalDevice, transferQueue.index, 0, &transferQueue.queue);

}

// TO DO
// Support multiple command pools
void Renderer::CreateCommandBuffers()
{

	std::set<int> queuefamilies{ graphicsQueue.index, presentQueue.index, transferQueue.index };
	VkCommandPoolCreateInfo poolinfo{};
	poolinfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolinfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolinfo.queueFamilyIndex = graphicsQueue.index;

	vkCreateCommandPool(logicalDevice, &poolinfo, nullptr, &cmdPool);

	cmdBuffers.resize(1);

	VkCommandBufferAllocateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	info.commandBufferCount = cmdBuffers.size();
	info.commandPool = cmdPool;
	info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

	vkAllocateCommandBuffers(logicalDevice, &info, cmdBuffers.data());

	
}

void Renderer::CreateRenderPass()
{
	VkAttachmentDescription att[2]{};
	att[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	att[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	att[0].format = presentationFormat.format;
	att[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	att[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	att[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	att[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	att[0].samples = VK_SAMPLE_COUNT_1_BIT;

	att[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	att[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	att[1].format = VK_FORMAT_D16_UNORM;
	att[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	att[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	att[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	att[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	att[1].samples = VK_SAMPLE_COUNT_1_BIT;

	VkAttachmentReference colorref;
	colorref.attachment = 0;
	colorref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthref;
	depthref.attachment = 0;
	depthref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkRenderPassCreateInfo info{};
	info.attachmentCount = 2;
	info.dependencyCount = 0;
	info.subpassCount = 0;
	info.pAttachments;
	info.pDependencies = nullptr;
	info.pSubpasses = nullptr;

}

void Renderer::CreatePipeline()
{

}

void Renderer::PrintLayers()
{
	uint32_t count;
	std::vector<VkLayerProperties> layerprops;
	vkEnumerateInstanceLayerProperties(&count, nullptr);
	layerprops.resize(count);
	vkEnumerateInstanceLayerProperties(&count, layerprops.data());

	for (VkLayerProperties p : layerprops)
	{
		std::cout << p.layerName << std::endl;
	}

	std::cout << std::endl;
}
void Renderer::PrintExtensions()
{
	uint32_t count;
	std::vector<VkExtensionProperties> properties;
	vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
	properties.resize(count);
	vkEnumerateInstanceExtensionProperties(nullptr, &count, properties.data());

	for (VkExtensionProperties p : properties)
	{
		std::cout << p.extensionName << std::endl;
	}

	std::cout << std::endl;
}
