#include "Window.hpp"

Window::Window()
{
}

Window::~Window()
{
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(window);
}

void Window::CreateSwapchain()
{
	VkResult result;

	VkDevice logicalDevice = renderer.GetDevice();
	DeviceQueue presentQueue = renderer.GetPresentQueue();
	DeviceQueue graphicsQueue = renderer.GetGraphicsQueue();
	VkPhysicalDevice physicalDevice = renderer.GetPhysicalDevice();
	VkPresentModeKHR chosenMode;
	uint32_t count;

	if (swapchain.chain != VK_NULL_HANDLE)
	{
		for (int i = 0; i < swapchain.images.size(); i++)
		{
			if (swapchain.images[i] != VK_NULL_HANDLE)
				vkDestroyImage(logicalDevice, swapchain.images[i], nullptr);
			if (swapchain.views[i] != VK_NULL_HANDLE)
				vkDestroyImageView(logicalDevice, swapchain.views[i], nullptr);
		}
		vkDestroySwapchainKHR(logicalDevice, swapchain.chain, nullptr);
	}

	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &count, nullptr);
	std::vector<VkSurfaceFormatKHR> formats(count);
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &count, formats.data());

	for (int i = 0; i < count; i++)
	{
		if (formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			swapchain.format = formats[i];
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
	info.imageColorSpace = swapchain.format.colorSpace;
	info.imageExtent = extent;
	info.imageFormat = swapchain.format.format;
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

	for (int i = 0; i < swapchain.images.size(); i++)
	{
		VkImageViewCreateInfo viewinfo{};
		viewinfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewinfo.components.r = VK_COMPONENT_SWIZZLE_R;
		viewinfo.components.g = VK_COMPONENT_SWIZZLE_G;
		viewinfo.components.b = VK_COMPONENT_SWIZZLE_B;
		viewinfo.components.a = VK_COMPONENT_SWIZZLE_A;
		viewinfo.format = swapchain.format.format;
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

void Window::CreateSurface()
{
	VkInstance inst = renderer.GetInstance();

	if (surface != VK_NULL_HANDLE)
		vkDestroySurfaceKHR(inst, surface,nullptr);

	glfwCreateWindowSurface(inst, window, nullptr, &surface);
}

void Window::CreateWindow()
{
	if (window != nullptr)
	{
		glfwDestroyWindow(window);
		window = nullptr;
	}

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	window = glfwCreateWindow(800, 800, "Default Title", nullptr, nullptr);
}

void Window::InitGLFW()
{
	if (!initialized)
	{
		glfwInit();
		initialized = true;
	}
}

bool Window::initialized(false);
