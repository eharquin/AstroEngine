#include "pipeline.hpp"

// std
#include <fstream>
#include <iostream>
#include <stdexcept>

Pipeline::Pipeline(const std::string& vertexFilepath, const std::string& fragmentFilepath)
{
	createGraphicsPipeline(vertexFilepath, fragmentFilepath);
}

Pipeline::~Pipeline()
{

}

std::vector<char> Pipeline::readFile(const std::string& filePath)
{
	// construct an ifstream object and open a file with the cursor at the end (ate) and in binary reader mode
	std::ifstream file{ filePath, std::ios::ate | std::ios::binary };

	if (!file.is_open())
		throw std::runtime_error{"failed to open file: " + filePath};

	// get file size by using the cursor at the end
	size_t fileSize = static_cast<size_t>(file.tellg());

	std::vector<char> buffer(fileSize);

	// seek back the cursor to the beginning
	file.seekg(0);

	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}

void Pipeline::createGraphicsPipeline(const std::string& vertexFilepath, const std::string& fragmentFilepath)
{
	std::vector<char> vertexShader = Pipeline::readFile(vertexFilepath);
	std::vector<char> fragmentShader = Pipeline::readFile(fragmentFilepath);

	std::cout << "vertex shader code size : " << vertexShader.size() << std::endl;
	std::cout << "fragment shader code size : " << fragmentShader.size() << std::endl;

}