#include "descriptors.hpp"

Descriptors::Descriptors(AgDevice& agDevice, VkDescriptorPoolSize* poolSizes, uint32_t poolSizeCount, uint32_t maxSets = 1000)
	: agDevice(agDevice)
{
	createDescriptorPool(poolSizes, poolSizeCount, maxSets);
}

Descriptors::~Descriptors()
{
	vkDestroyDescriptorPool(agDevice.getDevice(), pool, nullptr);
}

void Descriptors::createDescriptorPool(VkDescriptorPoolSize* poolSizes, uint32_t poolSizeCount, uint32_t maxSets)
{
	VkDescriptorPoolCreateInfo descriptorPoolInfo{};
	descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolInfo.pNext = nullptr; // optional
	descriptorPoolInfo.flags = 0; // optional
	descriptorPoolInfo.maxSets = agDevice.MAX_FRAMES_IN_FLIGHT;
	descriptorPoolInfo.poolSizeCount = 1;
	descriptorPoolInfo.pPoolSizes = poolSizes;

	if (vkCreateDescriptorPool(agDevice.getDevice(), &descriptorPoolInfo, nullptr, &pool) != VK_SUCCESS)
		throw std::runtime_error("failed to create global descriptor pool!");
}

void Descriptors::addDescriptorSet(VkDescriptorSetLayoutBinding* bindings, uint32_t bindingCount, uint32_t descriptorCount, VkDescriptorBufferInfo* bufferInfo)
{

}
