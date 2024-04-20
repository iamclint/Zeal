#pragma once
#include <string>
#include <vector>
namespace StringUtil
{
	bool caseInsensitive(const std::string& str1, const std::string& str2);
	std::vector<std::string> split(const std::string& str, const std::string& delim);
	bool tryParse(const std::string& str, int* result);
	bool tryParse(const std::string& str, float* result);
	std::string byteArrayToHexString(const char* byteArray, size_t length);
};

