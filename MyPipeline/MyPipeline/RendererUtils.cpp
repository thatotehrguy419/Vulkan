#include "RendererUtils.hpp"

uint32_t FindMemoryTypeIndex(PhysicalDevice physicalDevice, uint32_t typebits, VkMemoryPropertyFlags properties)
{
	for (uint32_t i = 0; i < physicalDevice.memProperties.memoryTypeCount; i++)
	{
		if ((typebits & (1 << i) && (physicalDevice.memProperties.memoryTypes[i].propertyFlags & properties) == properties))
			return i;
	}

	return -1;
}

std::vector<char> LoadShaderCode(const char * file)
{
	std::ifstream infile(file, std::ios::in | std::ios::ate | std::ios::binary);

	uint32_t size = infile.tellg();
	infile.seekg(infile.beg);

	std::vector<char> code(size);
	infile.read(code.data(), size);

	return code;
}

