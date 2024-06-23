#pragma once
#include <string>
#include <vector>
namespace Zeal
{
	namespace String
	{
		std::string trim_and_reduce_spaces(const std::string& input);
		bool compare_insensitive(const std::string& str1, const std::string& str2);
		std::vector<std::string> split(const std::string& str, const std::string& delim);
		bool tryParse(const std::string& str, int* result);
		bool tryParse(const std::string& str, float* result);
		std::string bytes_to_hex(const char* byteArray, size_t length);
		std::string replace(std::string input, const std::string& from, const std::string& to);
	};
}