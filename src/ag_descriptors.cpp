#include "ag_descriptors.hpp"


// ********************* Descriptor Pool *********************

AgDescriptorPool::AgDescriptorPool(AgDevice& agDevice, uint32_t poolSizeCount, VkDescriptorPoolSize* poolSizes, VkDescriptorPoolCreateFlags flags, uint32_t maxSets)
	: agDevice(agDevice)
{
	VkDescriptorPoolCreateInfo descriptorPoolInfo{};
	descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolInfo.pNext = nullptr; // optional
	descriptorPoolInfo.flags = flags;
	descriptorPoolInfo.maxSets = maxSets;
	descriptorPoolInfo.poolSizeCount = poolSizeCount;
	descriptorPoolInfo.pPoolSizes = poolSizes;

	if(vkCreateDescriptorPool(agDevice.getDevice(), &descriptorPoolInfo, nullptr, &pool) != VK_SUCCESS)
		throw std::runtime_error("failed to create descriptor pool!");
}

AgDescriptorPool::~AgDescriptorPool()
{
	vkDestroyDescriptorPool(agDevice.getDevice(), pool, nullptr);
}

bool AgDescriptorPool::allocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const
{
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = pool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &descriptorSetLayout;
	
	// TODO : Might want to create a "DescriptorPoolManager" class that handles this case, and builds
	// a new pool whenever an old pool fills up
	if (vkAllocateDescriptorSets(agDevice.getDevice(), &allocInfo, &descriptor) != VK_SUCCESS)
		return false;
	
	return true;
}

// ****************** Descriptor Set Layout ******************

AgDescriptorSetLayout::AgDescriptorSetLayout(AgDevice& agDevice, uint32_t bindingCount, VkDescriptorSetLayoutBinding* bindings, VkDescriptorSetLayoutCreateFlags flags)
	: agDevice(agDevice)
{

	for (int i = 0; i < bindingCount; i++)
	{
		assert(bindingsMap.count(bindings[i].binding) == 0 && "Binding already in use");
		bindingsMap[bindings[i].binding] = bindings[i];
	}

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
	descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutInfo.pNext = nullptr; // optional
	descriptorSetLayoutInfo.flags = flags;
	descriptorSetLayoutInfo.bindingCount = bindingCount;
	descriptorSetLayoutInfo.pBindings = bindings;

	if (vkCreateDescriptorSetLayout(agDevice.getDevice(), &descriptorSetLayoutInfo, nullptr, &descriptorSetLayout))
		throw std::runtime_error("failed to create descriptor set layout!");
}

AgDescriptorSetLayout::~AgDescriptorSetLayout()
{
	vkDestroyDescriptorSetLayout(agDevice.getDevice(), descriptorSetLayout, nullptr);
}

// ****************** Descriptor Set Writer ******************

AgDescriptorSetWriter::AgDescriptorSetWriter(AgDescriptorSetLayout& layout)
	: layout(layout)
{
}

AgDescriptorSetWriter::~AgDescriptorSetWriter()
{
}

void AgDescriptorSetWriter::writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo)
{
	auto& bindingDescription = layout.bindingsMap[binding];

	assert(bindingDescription.descriptorCount == 1 && "Binding single descriptor info, but binding expects multiple");

	VkWriteDescriptorSet descriptorWrite;
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.pNext = nullptr;
	descriptorWrite.dstBinding = bindingDescription.binding;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = bindingDescription.descriptorType;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pBufferInfo = bufferInfo;
	descriptorWrite.pImageInfo = nullptr; // optional
	descriptorWrite.pTexelBufferView = nullptr; // optional

	writes.push_back(descriptorWrite);
}

void AgDescriptorSetWriter::writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo)
{
	auto& bindingDescription = layout.bindingsMap[binding];

	assert(bindingDescription.descriptorCount == 1 && "Binding single descriptor info, but binding expects multiple");

	VkWriteDescriptorSet descriptorWrite;
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.pNext = nullptr;
	descriptorWrite.dstBinding = bindingDescription.binding;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = bindingDescription.descriptorType;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pBufferInfo = nullptr; // optional
	descriptorWrite.pImageInfo = imageInfo; // optional
	descriptorWrite.pTexelBufferView = nullptr; // optional

	writes.push_back(descriptorWrite);
}


void AgDescriptorSetWriter::overwrite(VkDescriptorSet& descriptorSet)
{
	for (int i = 0; i < writes.size(); i++)
	{
		writes.at(i).dstSet = descriptorSet;
	}

	vkUpdateDescriptorSets(layout.agDevice.getDevice(), writes.size(), writes.data(), 0, nullptr);
}