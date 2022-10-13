#include "shader.hpp"

// std
#include <fstream>
#include <iostream>

Shader::Shader(LogicalDevice& logicalDevice, const std::string& filepath)
	: logicalDevice(logicalDevice)
{
	std::vector<char> code = Shader::readFile(filepath);

	std::cout << "shader code size : " << code.size() << std::endl;

	createShaderModule(code);
}

Shader::~Shader()
{
	vkDestroyShaderModule(logicalDevice.getVkDevice(), shader, nullptr);

	std::cout << "destruct" << std::endl;
}

std::vector<char> Shader::readFile(const std::string& filePath)
{
	// construct an ifstream object and open a file with the cursor at the end (ate) and in binary reader mode
	std::ifstream file{ filePath, std::ios::ate | std::ios::binary };

	if (!file.is_open())
		throw std::runtime_error{ "failed to open file: " + filePath };

	// get file size by using the cursor at the end
	size_t fileSize = static_cast<size_t>(file.tellg());

	std::vector<char> buffer(fileSize);

	// seek back the cursor to the beginning
	file.seekg(0);

	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}

void Shader::createShaderModule(const std::vector<char>& code)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.pNext = nullptr; // optional
	createInfo.flags = 0; // optional
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	if (vkCreateShaderModule(logicalDevice.getVkDevice(), &createInfo, nullptr, &shader) != VK_SUCCESS)
		throw std::runtime_error("failed to create shader module!");
}
