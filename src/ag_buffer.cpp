// astro
#include "ag_buffer.hpp"

/**
 * Returns the minimum instance size required to be compatible with devices minOffsetAlignment
 *
 * @param instanceSize The size of an instance
 * @param minOffsetAlignment The minimum required alignment, in bytes, for the offset member (eg : minUniformBufferOffsetAlignment)
 *
 * @return VkDeviceSize of the instanvceSize with the alignment
 */
VkDeviceSize AgBuffer::getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment)
{
	if (minOffsetAlignment > 0)
	{
		return (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
	}

	return instanceSize;
}

AgBuffer::AgBuffer(AgDevice& agDevice, VkDeviceSize instanceSize, uint32_t instanceCount, VkImageUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignement)
	: agDevice(agDevice), instanceSize(instanceSize), instanceCount(instanceCount), usageFlags(usageFlags), memoryPropertyFlags(memoryPropertyFlags)
{
	alignmentSize = getAlignment(instanceSize, minOffsetAlignement);
	bufferSize = alignmentSize * instanceCount;
	agDevice.createBuffer(bufferSize, usageFlags, memoryPropertyFlags, buffer, memory);
}

AgBuffer::~AgBuffer()
{
	unmap();
	vkDestroyBuffer(agDevice.getDevice(), buffer, nullptr);
	vkFreeMemory(agDevice.getDevice(), memory, nullptr);
}

VkResult AgBuffer::map(VkDeviceSize size, VkDeviceSize offset)
{
	return vkMapMemory(agDevice.getDevice(), memory, offset, size, 0, &mapped);
}

void AgBuffer::unmap()
{
	if (mapped)
	{
		vkUnmapMemory(agDevice.getDevice(), memory);
		mapped = nullptr;
	}
}

void AgBuffer::write(void* data, VkDeviceSize size, VkDeviceSize offset)
{
	assert(mapped && "cannot write on unmap memory");

	if (size == VK_WHOLE_SIZE)
		memcpy(mapped, data, bufferSize);

	else
	{
		char* memOffset = (char*)mapped;
		memOffset += offset;
		memcpy(memOffset, data, size);
	}
}




