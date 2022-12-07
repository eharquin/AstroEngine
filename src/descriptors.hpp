#pragma once
#include "ag_device.hpp"

class Descriptors
{
public:
	Descriptors(AgDevice& agDevice, VkDescriptorPoolSize* poolSizes, uint32_t poolSizeCount, uint32_t maxSets = 1000);
	~Descriptors();

private:
	AgDevice& agDevice;

	VkDescriptorPool pool;
	
	void createDescriptorPool(VkDescriptorPoolSize* poolSizes, uint32_t poolSizeCount, uint32_t maxSets);

	void addDescriptorSet(VkDescriptorSetLayoutBinding* bindings, uint32_t bindingCount, uint32_t descriptorCount, VkDescriptorBufferInfo* bufferInfo);

};