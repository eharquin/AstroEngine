#pragma once

// astro
#include "ag_device.hpp"

class AgTexture
{
public:
	AgTexture(AgDevice& device, const std::string& textureFilePath);
	AgTexture(AgDevice& device);
	~AgTexture();

	void createTextureImage(const std::string& textureFilePath);
	void createTextureImage();
	void createTextureSampler();

	// test
	VkImageView getTextureImageView() const { return textureImageView; }
	VkSampler getTextureSampler() const { return textureSampler; }

private:
	AgDevice& device;
	VkImage textureImage;
	uint32_t mipLevels;
	VkDeviceMemory textureImageMemory;

	VkImageView textureImageView;
	VkSampler textureSampler;
};