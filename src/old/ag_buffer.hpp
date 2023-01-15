#pragma once

// astro
#include "ag_device.hpp"

class AgBuffer
{
public:
	AgBuffer(AgDevice& agDevice, VkDeviceSize instanceSize, uint32_t instanceCount, VkImageUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignement = 0);
	~AgBuffer();

	// delete copy constructors
	AgBuffer(const AgBuffer&) = delete;
	AgBuffer operator=(const AgBuffer&) = delete;

	VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
	void unmap();

	void write(void* data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

	VkBuffer getBuffer() const { return buffer; }
	VkDeviceSize getSize() const { return bufferSize; }
	VkDescriptorBufferInfo getDescriptorBufferInfo() const { return VkDescriptorBufferInfo{ buffer, 0, VK_WHOLE_SIZE }; }

private:
	static VkDeviceSize getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment);

	// astro
	AgDevice& agDevice;

	void* mapped = nullptr;
	VkBuffer buffer = VK_NULL_HANDLE;
	VkDeviceMemory memory = VK_NULL_HANDLE;


	VkDeviceSize bufferSize;
	VkDeviceSize instanceSize;
	uint32_t instanceCount;

	VkDeviceSize alignmentSize;

	VkBufferUsageFlags usageFlags;
	VkMemoryPropertyFlags memoryPropertyFlags;
};