//#define NDEBUG
#include "Renderer.hpp"
#include <iostream>
#include <set>
#include <algorithm>

#ifdef NDEBUG
bool enableValidation = false;
#else
bool enableValidation = true;

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

void Renderer::Init(Window& window)
{
	CreateInstance();

	if (enableValidation)
		CreateDebugCallback();

	PickPhysicalDevice();

	UpdateWindow(window);
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
	att[0].format = displayFormat.format;
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

void Renderer::UpdateWindow(Window & window)
{
	Swapchain chain;
	VkSurfaceKHR surface = window.GetWindowSurface();

	VkResult result;
	VkPresentModeKHR chosenMode;
	uint32_t count;

	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &count, nullptr);
	std::vector<VkSurfaceFormatKHR> formats(count);
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &count, formats.data());

	for (int i = 0; i < count; i++)
	{
		if (formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			chain.format = formats[i];
	}

	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &count, nullptr);
	std::vector<VkPresentModeKHR> presentModes(count);
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &count, presentModes.data());

	for (int i = 0; i < count; i++)
		if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
			chosenMode = presentModes[i];

	if (chosenMode != VK_PRESENT_MODE_MAILBOX_KHR)
		chosenMode = VK_PRESENT_MODE_FIFO_KHR;

	chosenMode = VK_PRESENT_MODE_FIFO_KHR;

	VkSurfaceCapabilitiesKHR capabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities);

	VkExtent2D extent;
	extent.width = std::max(capabilities.currentExtent.width, capabilities.minImageExtent.width);
	extent.height = std::max(capabilities.currentExtent.height, capabilities.minImageExtent.height);
	extent.width = std::min(capabilities.currentExtent.width, capabilities.maxImageExtent.width);
	extent.height = std::min(capabilities.currentExtent.height, capabilities.maxImageExtent.height);


	std::vector<uint32_t> queueFamilyIndices;
	VkSwapchainCreateInfoKHR info{};
	info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	info.clipped = true;
	info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	info.imageArrayLayers = 1;
	info.imageColorSpace = chain.format.colorSpace;
	info.imageExtent = extent;
	info.imageFormat = chain.format.format;
	info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	info.minImageCount = 2;
	assert(capabilities.maxImageCount == 0 || info.minImageCount <= capabilities.maxImageCount);

	if (graphicsQueue.index == presentQueue.index)
	{
		info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		info.pQueueFamilyIndices = nullptr;
		info.queueFamilyIndexCount = 0;
	}
	else
	{
		queueFamilyIndices.push_back(graphicsQueue.index);
		queueFamilyIndices.push_back(presentQueue.index);
		info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		info.pQueueFamilyIndices = queueFamilyIndices.data();
		info.queueFamilyIndexCount = queueFamilyIndices.size();
	}
	info.presentMode = chosenMode;
	info.preTransform = capabilities.currentTransform;
	info.surface = surface;

	result = vkCreateSwapchainKHR(logicalDevice, &info, nullptr, &chain.chain);
	assert(result == VK_SUCCESS);

	vkGetSwapchainImagesKHR(logicalDevice, chain.chain, &count, nullptr);
	chain.images.resize(count);
	chain.views.resize(count);
	vkGetSwapchainImagesKHR(logicalDevice, chain.chain, &count, chain.images.data());

	for (int i = 0; i < chain.images.size(); i++)
	{
		VkImageViewCreateInfo viewinfo{};
		viewinfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewinfo.components.r = VK_COMPONENT_SWIZZLE_R;
		viewinfo.components.g = VK_COMPONENT_SWIZZLE_G;
		viewinfo.components.b = VK_COMPONENT_SWIZZLE_B;
		viewinfo.components.a = VK_COMPONENT_SWIZZLE_A;
		viewinfo.format = chain.format.format;
		viewinfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewinfo.image = chain.images[i];
		viewinfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewinfo.subresourceRange.baseArrayLayer = 0;
		viewinfo.subresourceRange.baseMipLevel = 0;
		viewinfo.subresourceRange.layerCount = 1;
		viewinfo.subresourceRange.levelCount = 1;

		result = vkCreateImageView(logicalDevice, &viewinfo, nullptr, &chain.views[i]);
		assert(result == VK_SUCCESS);
	}

	window.CreateSurface(instance);
	window.SetSwapchain(chain);
}
