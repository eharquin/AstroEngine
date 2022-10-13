#pragma once
#include "logical_device.hpp"


class Shader
{
public:
	Shader(LogicalDevice& logicalDevice, const std::string& filepath);
	~Shader();

	VkShaderModule getVkShaderModule() { return shader; }

private:
	VkShaderModule shader;

	LogicalDevice& logicalDevice;

	static std::vector<char> readFile(const std::string& filePath);

	void createShaderModule(const std::vector<char>& code);
};