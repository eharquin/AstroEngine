#pragma once

// std
#include <string>
#include <vector>

class Pipeline
{
public:
	Pipeline(const std::string& vertexFilepath, const std::string& fragmentFilepath);
	~Pipeline();

private:
	static std::vector<char> readFile(const std::string& filePath);

	void createGraphicsPipeline(const std::string& vertexFilepath, const std::string& fragmentFilepath);

};