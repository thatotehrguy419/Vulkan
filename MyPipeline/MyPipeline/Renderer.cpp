#include "DebugMode.hpp"
#include <iostream>
#include <set>
#include <algorithm>
#include "Renderer.hpp"
#include <cassert>

#define RED glm::vec4(1,0,0,1)
#define GREEN glm::vec4(0,1,0,1)
#define BLUE glm::vec4(0,0,1,1)
#define YELLOW glm::vec4(1,1,0,1)
#define MAGENTA glm::vec4(1,0,1,1)
#define CYAN glm::vec4(0,1,1,1)
#define XYZ1(_x_, _y_, _z_) glm::vec4(_x_,_y_,_z_,1.0f)

glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
glm::mat4 view = glm::lookAt(glm::vec3(-5, 3, -10), glm::vec3(0, 0, 0), glm::vec3(0, -1, 0));
glm::mat4 model = glm::mat4(1.0f);
glm::mat4 clip = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, -1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.5f, 0.0f,
	0.0f, 0.0f, 0.5f, 1.0f
);
glm::mat4 MVP = clip * projection * view * model;

Vertex cube[36] =
{
	//green face
	{ XYZ1(-1,-1,-1), XYZ1(0.f, 1.f, 0.f) },
	{ XYZ1(1,-1,-1), XYZ1(0.f, 1.f, 0.f) },
	{ XYZ1(-1, 1,-1), XYZ1(0.f, 1.f, 0.f) },
	{ XYZ1(-1, 1,-1), XYZ1(0.f, 1.f, 0.f) },
	{ XYZ1(1,-1,-1), XYZ1(0.f, 1.f, 0.f) },
	{ XYZ1(1, 1,-1), XYZ1(0.f, 1.f, 0.f) },
	//blue face
	{ XYZ1(-1, 1, 1), XYZ1(0.f, 0.f, 1.f) },
	{ XYZ1(-1,-1, 1), XYZ1(0.f, 0.f, 1.f) },
	{ XYZ1(-1, 1,-1), XYZ1(0.f, 0.f, 1.f) },
	{ XYZ1(-1, 1,-1), XYZ1(0.f, 0.f, 1.f) },
	{ XYZ1(-1,-1, 1), XYZ1(0.f, 0.f, 1.f) },
	{ XYZ1(-1,-1,-1), XYZ1(0.f, 0.f, 1.f) },
	//yellow face
	{ XYZ1(1, 1, 1), XYZ1(1.f, 1.f, 0.f) },
	{ XYZ1(1, 1,-1), XYZ1(1.f, 1.f, 0.f) },
	{ XYZ1(1,-1, 1), XYZ1(1.f, 1.f, 0.f) },
	{ XYZ1(1,-1, 1), XYZ1(1.f, 1.f, 0.f) },
	{ XYZ1(1, 1,-1), XYZ1(1.f, 1.f, 0.f) },
	{ XYZ1(1,-1,-1), XYZ1(1.f, 1.f, 0.f) },
	//magenta face
	{ XYZ1(1, 1, 1), XYZ1(1.f, 0.f, 1.f) },
	{ XYZ1(-1, 1, 1), XYZ1(1.f, 0.f, 1.f) },
	{ XYZ1(1, 1,-1), XYZ1(1.f, 0.f, 1.f) },
	{ XYZ1(1, 1,-1), XYZ1(1.f, 0.f, 1.f) },
	{ XYZ1(-1, 1, 1), XYZ1(1.f, 0.f, 1.f) },
	{ XYZ1(-1, 1,-1), XYZ1(1.f, 0.f, 1.f) },
	//cyan face
	{ XYZ1(1,-1, 1), XYZ1(0.f, 1.f, 1.f) },
	{ XYZ1(1,-1,-1), XYZ1(0.f, 1.f, 1.f) },
	{ XYZ1(-1,-1, 1), XYZ1(0.f, 1.f, 1.f) },
	{ XYZ1(-1,-1, 1), XYZ1(0.f, 1.f, 1.f) },
	{ XYZ1(1,-1,-1), XYZ1(0.f, 1.f, 1.f) },
	{ XYZ1(-1,-1,-1), XYZ1(0.f, 1.f, 1.f) },
	//red face
	{ XYZ1(-1,-1, 1), XYZ1(1.f, 0.f, 0.f) },
	{ XYZ1(-1, 1, 1), XYZ1(1.f, 0.f, 0.f) },
	{ XYZ1(1,-1, 1), XYZ1(1.f, 0.f, 0.f) },
	{ XYZ1(1,-1, 1), XYZ1(1.f, 0.f, 0.f) },
	{ XYZ1(-1, 1, 1), XYZ1(1.f, 0.f, 0.f) },
	{ XYZ1(1, 1, 1), XYZ1(1.f, 0.f, 0.f) },
};

#ifdef NDEBUG
extern bool const enableValidation = false;

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
	return VK_TRUE;
}
#else
extern bool const enableValidation = true;

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

Renderer::Renderer()
{

}

Renderer::Renderer(Renderer &other)
{

}

Renderer::~Renderer()
{
	DestroyPipeline();
	DestroyShaderModules();
	DestroyBuffers();
	DestroyDescriptors();
	DestroyDepthImage();
	DestroyRenderPass();
	DestroyCommandPools();
	DestroySwapChain();
	DestroyValidationCallback();
	DestroySurface();
	DestroyDevice();
	DestroyInstance();
	
}

void Renderer::Render()
{
	VkSemaphoreCreateInfo seminfo{};
	seminfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	vkCreateSemaphore(logicalDevice, &seminfo, nullptr, &imageacquired);

	uint32_t index;
	vkAcquireNextImageKHR(logicalDevice, swapchain.chain, UINT64_MAX, imageacquired, VK_NULL_HANDLE, &index);

	VkCommandBufferBeginInfo cmdbegin{};
	cmdbegin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	vkBeginCommandBuffer(graphicsCmdBuffers[index], &cmdbegin);

	VkClearValue clearvals[2];
	clearvals[0].color.float32[0] = 0.4f;
	clearvals[0].color.float32[1] = 0.4f;
	clearvals[0].color.float32[2] = 0.4f;
	clearvals[0].color.float32[3] = 1.0f;
	clearvals[1].depthStencil.depth = 1.0f;
	clearvals[1].depthStencil.stencil = 0;


	VkRenderPassBeginInfo rpbegin{};
	rpbegin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	rpbegin.framebuffer = frameBuffers[index];
	rpbegin.renderPass = renderPass;
	rpbegin.renderArea.offset = { 0,0 };
	rpbegin.renderArea.extent = swapchain.extent;
	rpbegin.clearValueCount = 2;
	rpbegin.pClearValues = clearvals;

	vkCmdBeginRenderPass(graphicsCmdBuffers[index], &rpbegin, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(graphicsCmdBuffers[index], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
	vkCmdBindDescriptorSets(graphicsCmdBuffers[index], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
	
	VkDeviceSize offset = 0;
	vkCmdBindVertexBuffers(graphicsCmdBuffers[index], 0, 1, &vertexBuffer.buffer, &offset);
	vkCmdDraw(graphicsCmdBuffers[index], 24, 0, 0, 0);
	vkEndCommandBuffer(graphicsCmdBuffers[index]);

	VkSubmitInfo submitinfo{};
	submitinfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitinfo.pCommandBuffers = &graphicsCmdBuffers[index];
	submitinfo.commandBufferCount = 1;

	vkQueueSubmit(graphicsQueue.queue, 1, &submitinfo, VK_NULL_HANDLE);

	VkPresentInfoKHR presentinfo{};
	presentinfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentinfo.pSwapchains = &swapchain.chain;
	presentinfo.swapchainCount = 1;
	presentinfo.pImageIndices = &index;

	vkQueuePresentKHR(presentQueue.queue, &presentinfo);
}

////////////////////////////////////////////////////////////////////////////////////////////
////									CONSTRUCTION									////
////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::CreateInstance()
{

	VkResult result;
	std::vector<const char*> layers;
	std::vector<const char*> extensions;
	VkApplicationInfo appinfo{};
	VkInstanceCreateInfo info{};
	uint32_t count;
	const char** windowExts;

	DestroyInstance();

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
			vkGetPhysicalDeviceSurfaceSupportKHR(d, i, surface, &supportSurface);

			if (supportSurface)
				presentQueue.index = i;

			if (qfamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT && i != graphicsQueue.index)
				transferQueue.index = i;

			if (graphicsQueue.index > -1 && presentQueue.index > -1 && transferQueue.index > -1)
			{
				physicalDevice.device = d;
				break;
			}
		}

		if (graphicsQueue.index > -1 && transferQueue.index == -1 && qfamilies[graphicsQueue.index].queueFlags & VK_QUEUE_TRANSFER_BIT)
		{
			transferQueue.index = graphicsQueue.index;
			physicalDevice.device = d;
		}

		assert(physicalDevice.device != VK_NULL_HANDLE);

		vkGetPhysicalDeviceMemoryProperties(physicalDevice.device, &physicalDevice.memProperties);
		vkGetPhysicalDeviceFeatures(physicalDevice.device, &physicalDevice.features);

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

	result = vkCreateDevice(physicalDevice.device, &info, nullptr, &logicalDevice);
	assert(result == VK_SUCCESS);


	vkGetDeviceQueue(logicalDevice, graphicsQueue.index, 0, &graphicsQueue.queue);
	vkGetDeviceQueue(logicalDevice, presentQueue.index, 0, &presentQueue.queue);
	vkGetDeviceQueue(logicalDevice, transferQueue.index, 0, &transferQueue.queue);

}

void Renderer::CreateSurface(GLFWwindow* window)
{
	DestroySurface();

	glfwCreateWindowSurface(instance, window, nullptr, &surface);
}

void Renderer::CreateCommandBuffers()
{
	VkResult result;
	VkCommandPoolCreateInfo poolinfo{};

	DestroyCommandPools();

	// Graphics Command Pool
	poolinfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolinfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT | VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
	poolinfo.queueFamilyIndex = graphicsQueue.index;

	result = vkCreateCommandPool(logicalDevice, &poolinfo, nullptr, &graphicsCmdPool);
	assert(result == VK_SUCCESS);

	graphicsCmdBuffers.resize(1);

	VkCommandBufferAllocateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	info.commandBufferCount = graphicsCmdBuffers.size();
	info.commandPool = graphicsCmdPool;
	info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

	vkAllocateCommandBuffers(logicalDevice, &info, graphicsCmdBuffers.data());
	assert(result == VK_SUCCESS);

	// Transfer Command Pool
	poolinfo.flags = 0;
	poolinfo.queueFamilyIndex = transferQueue.index;

	result = vkCreateCommandPool(logicalDevice, &poolinfo, nullptr, &transferCmdPool);
	assert(result == VK_SUCCESS);

	transferCmdBuffers.resize(1);

	info.commandBufferCount = transferCmdBuffers.size();
	info.commandPool = transferCmdPool;
	info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

	vkAllocateCommandBuffers(logicalDevice, &info, transferCmdBuffers.data());
	assert(result == VK_SUCCESS);

}

void Renderer::CreateSwapchain()
{
	VkResult result;

	VkPresentModeKHR chosenMode;
	uint32_t count;

	DestroySwapChain();

	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice.device, surface, &count, nullptr);
	std::vector<VkSurfaceFormatKHR> formats(count);
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice.device, surface, &count, formats.data());

	for (uint32_t i = 0; i < count; i++)
	{
		if (formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			swapchain.surfaceFormat = formats[i];
	}

	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice.device, surface, &count, nullptr);
	std::vector<VkPresentModeKHR> presentModes(count);
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice.device, surface, &count, presentModes.data());

	for (uint32_t i = 0; i < count; i++)
		if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
			chosenMode = presentModes[i];

	if (chosenMode != VK_PRESENT_MODE_MAILBOX_KHR)
		chosenMode = VK_PRESENT_MODE_FIFO_KHR;

	chosenMode = VK_PRESENT_MODE_FIFO_KHR;

	VkSurfaceCapabilitiesKHR capabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice.device, surface, &capabilities);

	swapchain.extent.width = std::max(capabilities.currentExtent.width, capabilities.minImageExtent.width);
	swapchain.extent.height = std::max(capabilities.currentExtent.height, capabilities.minImageExtent.height);
	swapchain.extent.width = std::min(capabilities.currentExtent.width, capabilities.maxImageExtent.width);
	swapchain.extent.height = std::min(capabilities.currentExtent.height, capabilities.maxImageExtent.height);


	std::vector<uint32_t> queueFamilyIndices;
	VkSwapchainCreateInfoKHR info{};
	info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	info.clipped = true;
	info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	info.imageArrayLayers = 1;
	info.imageColorSpace = swapchain.surfaceFormat.colorSpace;
	info.imageExtent = swapchain.extent;
	info.imageFormat = swapchain.surfaceFormat.format;
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

	result = vkCreateSwapchainKHR(logicalDevice, &info, nullptr, &swapchain.chain);
	assert(result == VK_SUCCESS);

	vkGetSwapchainImagesKHR(logicalDevice, swapchain.chain, &count, nullptr);
	swapchain.images.resize(count);
	swapchain.views.resize(count);
	vkGetSwapchainImagesKHR(logicalDevice, swapchain.chain, &count, swapchain.images.data());

	for (uint32_t i = 0; i < swapchain.images.size(); i++)
	{
		VkImageViewCreateInfo viewinfo{};
		viewinfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewinfo.components.r = VK_COMPONENT_SWIZZLE_R;
		viewinfo.components.g = VK_COMPONENT_SWIZZLE_G;
		viewinfo.components.b = VK_COMPONENT_SWIZZLE_B;
		viewinfo.components.a = VK_COMPONENT_SWIZZLE_A;
		viewinfo.format = swapchain.surfaceFormat.format;
		viewinfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewinfo.image = swapchain.images[i];
		viewinfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewinfo.subresourceRange.baseArrayLayer = 0;
		viewinfo.subresourceRange.baseMipLevel = 0;
		viewinfo.subresourceRange.layerCount = 1;
		viewinfo.subresourceRange.levelCount = 1;

		result = vkCreateImageView(logicalDevice, &viewinfo, nullptr, &swapchain.views[i]);
		assert(result == VK_SUCCESS);
	}
}

void Renderer::CreateRenderPass()
{
	VkResult result;

	DestroyRenderPass();
	CreateDepthImage();

	VkAttachmentDescription att[2]{};
	att[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	att[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	att[0].format = swapchain.surfaceFormat.format;
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
	depthref.attachment = 1;
	depthref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorref;
	subpass.pDepthStencilAttachment = &depthref;

	VkRenderPassCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	info.attachmentCount = 2;
	info.dependencyCount = 0;
	info.subpassCount = 1;
	info.pAttachments = att;
	info.pSubpasses = &subpass;

	result = vkCreateRenderPass(logicalDevice, &info, nullptr, &renderPass);
	assert(result == VK_SUCCESS);

	frameBuffers.resize(swapchain.views.size());

	VkImageView attviews[2]{};
	attviews[1] = depthImage.view;

	for (int i = 0; i < swapchain.views.size(); i++)
	{
		attviews[0] = swapchain.views[i];

		VkFramebufferCreateInfo fbinfo{};
		fbinfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		fbinfo.attachmentCount = 2;
		fbinfo.pAttachments = attviews;
		fbinfo.renderPass = renderPass;
		fbinfo.layers = 1;
		fbinfo.height = swapchain.extent.height;
		fbinfo.width = swapchain.extent.width;

		result = vkCreateFramebuffer(logicalDevice, &fbinfo, nullptr, &frameBuffers[i]);
		assert(result == VK_SUCCESS);
	}
}

void Renderer::CreatePipeline()
{
	VkResult result;

	CreateDescriptors();
	CreatePipelineBuffers();
	CreateShaderModule(&vertexModule, "Shaders/vert.spv");
	CreateShaderModule(&fragmentModule, "Shaders/frag.spv");

	VkPipelineLayoutCreateInfo layoutinfo{};
	layoutinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	layoutinfo.setLayoutCount = 1;
	layoutinfo.pSetLayouts = &descriptorLayout;

	result = vkCreatePipelineLayout(logicalDevice, &layoutinfo, nullptr, &pipelineLayout);
	assert(result == VK_SUCCESS);

	VkPipelineColorBlendAttachmentState cbstate{};
	cbstate.blendEnable = VK_FALSE;
	cbstate.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

	VkPipelineColorBlendStateCreateInfo colorblendinfo{};
	colorblendinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorblendinfo.attachmentCount = 1;
	colorblendinfo.pAttachments = &cbstate;
	colorblendinfo.logicOpEnable = VK_FALSE;
	colorblendinfo.logicOp = VK_LOGIC_OP_NO_OP;
	colorblendinfo.blendConstants[0] = 0;
	colorblendinfo.blendConstants[1] = 0;
	colorblendinfo.blendConstants[2] = 0;
	colorblendinfo.blendConstants[3] = 0;

	VkPipelineDepthStencilStateCreateInfo depthstencilinfo{};
	depthstencilinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthstencilinfo.stencilTestEnable = VK_FALSE;
	depthstencilinfo.depthWriteEnable = VK_TRUE;
	depthstencilinfo.depthBoundsTestEnable = VK_FALSE;
	depthstencilinfo.depthTestEnable = VK_TRUE;
	depthstencilinfo.depthCompareOp = VK_COMPARE_OP_LESS;
	depthstencilinfo.back.failOp = VK_STENCIL_OP_KEEP;
	depthstencilinfo.back.passOp = VK_STENCIL_OP_KEEP;
	depthstencilinfo.back.compareOp = VK_COMPARE_OP_ALWAYS;
	depthstencilinfo.back.compareMask = 0;
	depthstencilinfo.back.reference = 0;
	depthstencilinfo.back.writeMask = 0;
	depthstencilinfo.front = depthstencilinfo.back;

	VkPipelineInputAssemblyStateCreateInfo inputassemblyinfo{};
	inputassemblyinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputassemblyinfo.primitiveRestartEnable = VK_FALSE;
	inputassemblyinfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

	VkVertexInputBindingDescription bindingdescription;
	bindingdescription.binding = 0;
	bindingdescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	bindingdescription.stride = sizeof(Vertex);
	VkVertexInputAttributeDescription attribdescription[2];
	attribdescription[0].binding = 0;
	attribdescription[0].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	attribdescription[0].location = 0;
	attribdescription[0].offset = 0;

	attribdescription[1].binding = 0;
	attribdescription[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	attribdescription[1].location = 1;
	attribdescription[1].offset = sizeof(glm::vec4);
	VkPipelineVertexInputStateCreateInfo vertexinputinfo{};
	vertexinputinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexinputinfo.vertexAttributeDescriptionCount = 2;
	vertexinputinfo.vertexBindingDescriptionCount = 1;
	vertexinputinfo.pVertexAttributeDescriptions = attribdescription;
	vertexinputinfo.pVertexBindingDescriptions = &bindingdescription;

	VkPipelineMultisampleStateCreateInfo multisampleinfo{};
	multisampleinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampleinfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampleinfo.alphaToCoverageEnable = VK_FALSE;
	multisampleinfo.alphaToOneEnable = VK_FALSE;
	multisampleinfo.sampleShadingEnable = VK_FALSE;

	VkPipelineRasterizationStateCreateInfo rasterinfo{};
	rasterinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterinfo.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterinfo.depthBiasEnable = VK_FALSE;
	rasterinfo.depthClampEnable = VK_FALSE;
	rasterinfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterinfo.polygonMode = VK_POLYGON_MODE_FILL;
	rasterinfo.lineWidth = 1;
	rasterinfo.rasterizerDiscardEnable = VK_FALSE;


	std::vector<VkDynamicState> dynamicstate= { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
	VkPipelineDynamicStateCreateInfo dynamicinfo{};
	dynamicinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicinfo.dynamicStateCount = dynamicstate.size();
	dynamicinfo.pDynamicStates = dynamicstate.data();

	VkPipelineShaderStageCreateInfo stages[2];
	stages[0] = {};
	stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	stages[0].module = vertexModule;
	stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	stages[0].pName = "main";

	stages[1] = {};
	stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	stages[1].module = fragmentModule;
	stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	stages[1].pName = "main";


	VkViewport viewport;
	viewport.x = 0;
	viewport.y = 0;
	viewport.width = swapchain.extent.width;
	viewport.height = swapchain.extent.height;
	viewport.minDepth = 0.0;
	viewport.maxDepth = 1.0;
	VkRect2D scissor;
	scissor.extent = swapchain.extent;
	scissor.offset = {0,0};
	VkPipelineViewportStateCreateInfo viewportstate{};
	viewportstate.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportstate.pScissors = &scissor;
	viewportstate.scissorCount = 1;
	viewportstate.pViewports = &viewport;
	viewportstate.viewportCount = 1;

	VkGraphicsPipelineCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	info.basePipelineHandle = VK_NULL_HANDLE;
	info.basePipelineIndex = 0;
	info.layout = pipelineLayout;
	info.pColorBlendState = &colorblendinfo;
	info.pDepthStencilState = &depthstencilinfo;
	info.pDynamicState = &dynamicinfo;
	info.pInputAssemblyState = &inputassemblyinfo;
	info.pMultisampleState = &multisampleinfo;
	info.pRasterizationState = &rasterinfo;
	info.pTessellationState = nullptr;
	info.pVertexInputState = &vertexinputinfo;
	info.pViewportState = &viewportstate;
	info.renderPass = renderPass;
	info.subpass = 0;
	info.pStages = stages;
	info.stageCount = 2;

	result = vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &info, nullptr, &graphicsPipeline);
	assert(result == VK_SUCCESS);
}

void Renderer::CreatePipelineBuffers()
{
	VkResult result;
	uint32_t qfinidices[2] = { (uint32_t)graphicsQueue.index, (uint32_t)transferQueue.index };

	vkDeviceWaitIdle(logicalDevice);

	DestroyBuffers();

	VkDeviceSize size;
	std::set<uint32_t> indices;

	indices.insert(transferQueue.index);
	size = sizeof(cube);
	CreateBuffer(&vertexStagingBuffer, size,VK_BUFFER_USAGE_TRANSFER_SRC_BIT, indices);

	indices.clear();
	indices.insert(graphicsQueue.index);
	indices.insert(transferQueue.index);
	size = sizeof(cube);
	CreateBuffer(&vertexBuffer, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, indices );

	indices.clear();
	indices.insert(graphicsQueue.index);
	size = sizeof(MVP);
	CreateBuffer(&uniformBuffer, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, indices);

}

void Renderer::CreateDepthImage()
{
	VkResult result;

	DestroyDepthImage();

	VkImageCreateInfo depthinfo{};
	depthinfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	depthinfo.extent = { swapchain.extent.width, swapchain.extent.height, 1 };
	depthinfo.format = VK_FORMAT_D16_UNORM;
	depthinfo.imageType = VK_IMAGE_TYPE_2D;
	depthinfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthinfo.mipLevels = 1;
	depthinfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	depthinfo.samples = VK_SAMPLE_COUNT_1_BIT;
	depthinfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	depthinfo.pQueueFamilyIndices = (uint32_t*)&graphicsQueue.index;
	depthinfo.queueFamilyIndexCount = 1;
	depthinfo.arrayLayers = 1;

	result = vkCreateImage(logicalDevice, &depthinfo, nullptr, &depthImage.image);
	assert(result == VK_SUCCESS);

	VkMemoryRequirements memreqs;
	VkMemoryAllocateInfo allocinfo{};

	vkGetImageMemoryRequirements(logicalDevice, depthImage.image, &memreqs);

	allocinfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocinfo.allocationSize = memreqs.size;
	allocinfo.memoryTypeIndex = FindMemoryTypeIndex(physicalDevice, memreqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	result = vkAllocateMemory(logicalDevice, &allocinfo, nullptr, &depthImage.memory);
	assert(result == VK_SUCCESS);
	result - vkBindImageMemory(logicalDevice, depthImage.image, depthImage.memory, 0);
	assert(result == VK_SUCCESS);

	VkImageViewCreateInfo dviewinfo{};
	dviewinfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	dviewinfo.format = VK_FORMAT_D16_UNORM;
	dviewinfo.image = depthImage.image;
	dviewinfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	dviewinfo.components.r = VK_COMPONENT_SWIZZLE_R;
	dviewinfo.components.g = VK_COMPONENT_SWIZZLE_G;
	dviewinfo.components.b = VK_COMPONENT_SWIZZLE_B;
	dviewinfo.components.a = VK_COMPONENT_SWIZZLE_A;
	dviewinfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	dviewinfo.subresourceRange.baseArrayLayer = 0;
	dviewinfo.subresourceRange.baseMipLevel = 0;
	dviewinfo.subresourceRange.layerCount = 1;
	dviewinfo.subresourceRange.levelCount = 1;

	result = vkCreateImageView(logicalDevice, &dviewinfo, nullptr, &depthImage.view);
	assert(result == VK_SUCCESS);


}

void Renderer::CreateDescriptors()
{
	VkResult result;

	VkDescriptorSetLayoutBinding binding{};
	binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	binding.descriptorCount = 1;
	binding.pImmutableSamplers = nullptr;
	binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	VkDescriptorSetLayoutCreateInfo dslayout{};
	dslayout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	dslayout.bindingCount = 1;
	dslayout.pBindings = &binding;

	result = vkCreateDescriptorSetLayout(logicalDevice, &dslayout, nullptr, &descriptorLayout);
	assert(result == VK_SUCCESS);

	VkDescriptorPoolSize size{};
	size.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	size.descriptorCount = 1;

	VkDescriptorPoolCreateInfo poolinfo{};
	poolinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolinfo.poolSizeCount = 1;
	poolinfo.pPoolSizes = &size;
	poolinfo.maxSets = 1;

	result = vkCreateDescriptorPool(logicalDevice,&poolinfo,nullptr, &descriptorPool);
	assert(result == VK_SUCCESS);


	VkDescriptorSetAllocateInfo allocinfo{};
	allocinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocinfo.descriptorPool = descriptorPool;
	allocinfo.descriptorSetCount = 1;
	allocinfo.pSetLayouts = &descriptorLayout;

	result = vkAllocateDescriptorSets(logicalDevice, &allocinfo, &descriptorSet);
	assert(result == VK_SUCCESS);

	VkDescriptorBufferInfo buffinfo{};
	buffinfo.buffer = uniformBuffer.buffer;
	buffinfo.offset = 0;
	buffinfo.range = VK_WHOLE_SIZE;

	VkWriteDescriptorSet write{};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.descriptorCount = 1;
	write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	write.dstBinding = 0;
	write.dstArrayElement = 0;
	write.dstSet = 0;
	write.pBufferInfo = &buffinfo;

	vkUpdateDescriptorSets(logicalDevice, 1, &write,0,nullptr);
}

void Renderer::CreateBuffer(Buffer* buffer, VkDeviceSize size, VkBufferUsageFlags usage, std::set<uint32_t> set)
{
	VkResult result;

	std::vector<uint32_t> indices(set.size());
	for (uint32_t i : set)
	{
		indices.push_back(i);
	}

	VkBufferCreateInfo createinfo{};
	createinfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	createinfo.pQueueFamilyIndices = indices.data();
	createinfo.queueFamilyIndexCount = indices.size();
	createinfo.size = size;
	createinfo.usage = usage;
	if (indices.size() == 1)
		createinfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	else
		createinfo.sharingMode = VK_SHARING_MODE_CONCURRENT;

	result = vkCreateBuffer(logicalDevice, &createinfo, nullptr, &buffer->buffer);
	assert(result == VK_SUCCESS);

	VkMemoryRequirements memreqs;
	VkMemoryAllocateInfo allocinfo{};

	// Vertex Staging Buffer
	vkGetBufferMemoryRequirements(logicalDevice, buffer->buffer, &memreqs);

	allocinfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocinfo.allocationSize = memreqs.size;
	allocinfo.memoryTypeIndex = FindMemoryTypeIndex(physicalDevice, memreqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

	result = vkAllocateMemory(logicalDevice, &allocinfo, nullptr, &buffer->memory);
	assert(result == VK_SUCCESS);

	vkBindBufferMemory(logicalDevice, buffer->buffer, buffer->memory, 0);

}

void Renderer::CreateShaderModule(VkShaderModule* module, const char * file)
{
	std::vector<char> code = LoadShaderCode(file);

	VkShaderModuleCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	info.pCode = (uint32_t*)code.data();
	info.codeSize = code.size();	
	
	vkCreateShaderModule(logicalDevice, &info, nullptr, module);

}

////////////////////////////////////////////////////////////////////////////////////////////
////									DESTRUCTION										////
////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::DestroyInstance()
{
	if (instance != VK_NULL_HANDLE)
		vkDestroyInstance(instance, nullptr);
	instance = VK_NULL_HANDLE;
}

void Renderer::DestroyValidationCallback()
{
	if (debugCallback != VK_NULL_HANDLE)
		DestroyCallback(instance, debugCallback, nullptr);

	debugCallback = VK_NULL_HANDLE;
}

void Renderer::DestroyDevice()
{
	if (logicalDevice != VK_NULL_HANDLE)
		vkDestroyDevice(logicalDevice, nullptr);

	logicalDevice = VK_NULL_HANDLE;
}

void Renderer::DestroySurface()
{
	if (surface != VK_NULL_HANDLE)
		vkDestroySurfaceKHR(instance, surface, nullptr);

	surface = VK_NULL_HANDLE;
}

void Renderer::DestroySwapChain()
{
	if (swapchain.chain != VK_NULL_HANDLE)
	{
		vkDestroySwapchainKHR(logicalDevice, swapchain.chain, nullptr);
		for (VkImageView view : swapchain.views)
		{
			vkDestroyImageView(logicalDevice, view, nullptr);
		}
	}
	swapchain.chain = VK_NULL_HANDLE;
}

void Renderer::DestroyCommandPools()
{
	if (graphicsCmdPool != VK_NULL_HANDLE)
		vkDestroyCommandPool(logicalDevice, graphicsCmdPool, nullptr);
	if (transferCmdPool != VK_NULL_HANDLE)
		vkDestroyCommandPool(logicalDevice, transferCmdPool, nullptr);

	graphicsCmdPool = VK_NULL_HANDLE;
	transferCmdPool = VK_NULL_HANDLE;
}

void Renderer::DestroyDescriptors()
{
	if (descriptorLayout != VK_NULL_HANDLE)
		vkDestroyDescriptorSetLayout(logicalDevice, descriptorLayout, nullptr);

	if (descriptorPool != VK_NULL_HANDLE)
		vkDestroyDescriptorPool(logicalDevice, descriptorPool, nullptr);

	descriptorLayout = VK_NULL_HANDLE;
	descriptorPool = VK_NULL_HANDLE;
}

void Renderer::DestroyDepthImage()
{
	if (depthImage.image != VK_NULL_HANDLE)
		vkDestroyImageView(logicalDevice, depthImage.view, nullptr);

	if(depthImage.view != VK_NULL_HANDLE)
		vkDestroyImage(logicalDevice, depthImage.image, nullptr);

	if (depthImage.memory != VK_NULL_HANDLE)
		vkFreeMemory(logicalDevice, depthImage.memory, nullptr);

	depthImage.image = VK_NULL_HANDLE;
	depthImage.view = VK_NULL_HANDLE;
	depthImage.memory = VK_NULL_HANDLE;
}

void Renderer::DestroyRenderPass()
{
	if (renderPass != VK_NULL_HANDLE)
		vkDestroyRenderPass(logicalDevice, renderPass, nullptr);

	if (frameBuffers.size() != 0)
		for (size_t i = 0; i < frameBuffers.size(); i++)
		{
			vkDestroyFramebuffer(logicalDevice, frameBuffers[i], nullptr);
		}

	renderPass = VK_NULL_HANDLE;
	frameBuffers.clear();
}

void Renderer::DestroyBuffers()
{
	if (uniformBuffer.buffer != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(logicalDevice, uniformBuffer.buffer, nullptr);
		vkFreeMemory(logicalDevice, uniformBuffer.memory, nullptr);
	}

	if (vertexBuffer.buffer != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(logicalDevice, vertexBuffer.buffer, nullptr);
		vkFreeMemory(logicalDevice, vertexBuffer.memory, nullptr);
	}

	if (vertexStagingBuffer.buffer != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(logicalDevice, vertexStagingBuffer.buffer, nullptr);
		vkFreeMemory(logicalDevice, vertexStagingBuffer.memory, nullptr);
	}

	uniformBuffer.buffer = VK_NULL_HANDLE;
	uniformBuffer.memory = VK_NULL_HANDLE;
	vertexBuffer.buffer = VK_NULL_HANDLE;
	vertexBuffer.memory = VK_NULL_HANDLE;
	vertexStagingBuffer.buffer = VK_NULL_HANDLE;
	vertexStagingBuffer.memory = VK_NULL_HANDLE;
}

void Renderer::DestroyShaderModules()
{
	if (vertexModule != VK_NULL_HANDLE)
		vkDestroyShaderModule(logicalDevice, vertexModule, nullptr);

	if (fragmentModule != VK_NULL_HANDLE)
		vkDestroyShaderModule(logicalDevice, fragmentModule, nullptr);

	vertexModule = VK_NULL_HANDLE;
	fragmentModule = VK_NULL_HANDLE;
}

void Renderer::DestroyPipeline()
{
	if (pipelineLayout != VK_NULL_HANDLE)
		vkDestroyPipelineLayout(logicalDevice, pipelineLayout, nullptr);

	if (graphicsPipeline != VK_NULL_HANDLE)
		vkDestroyPipeline(logicalDevice, graphicsPipeline, nullptr);

	pipelineLayout = VK_NULL_HANDLE;
	graphicsPipeline = VK_NULL_HANDLE;
}


////////////////////////////////////////////////////////////////////////////////////////////
////									DEBUGGING										////
////////////////////////////////////////////////////////////////////////////////////////////

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
