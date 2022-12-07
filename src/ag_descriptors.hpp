#pragma once
// astro
#include "ag_device.hpp"
#include "ag_buffer.hpp"

//std
#include <unordered_map>

// ********************* Descriptor Pool *********************

class AgDescriptorPool
{
public:
	AgDescriptorPool(AgDevice& agDevice, uint32_t poolSizeCount, VkDescriptorPoolSize* poolSizes, VkDescriptorPoolCreateFlags flags, uint32_t maxSets);
	~AgDescriptorPool();

	VkDescriptorPool getPool() const { return pool; }

	bool allocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

private:
	// astro ref
	AgDevice& agDevice;

	VkDescriptorPool pool = VK_NULL_HANDLE;

};

// ****************** Descriptor Set Layout ******************

class AgDescriptorSetLayout
{
public:
	AgDescriptorSetLayout(AgDevice& agDevice, uint32_t bindingCount, VkDescriptorSetLayoutBinding* bindings, VkDescriptorSetLayoutCreateFlags flags = 0);
	~AgDescriptorSetLayout();

	VkDescriptorSetLayout getLayout() const { return descriptorSetLayout; }

private:
	// astro ref
	AgDevice& agDevice;
	
	VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;

	std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindingsMap{};

	friend class AgDescriptorSetWriter;
};

// ****************** Descriptor Set Writer ******************

class AgDescriptorSetWriter
{
public:
	AgDescriptorSetWriter(AgDescriptorSetLayout& layout);
	~AgDescriptorSetWriter();
	
	void writeBuffer(uint32_t binding, VkDescriptorBufferInfo bufferInfo);
	void writeImage();
	void writeTexel();

	void overwrite(VkDescriptorSet& descriptorSet);

private:
	// astro ref
	AgDescriptorSetLayout& layout;

	std::vector<VkWriteDescriptorSet> writes;
};