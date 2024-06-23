#include "string_util.h"
#include <algorithm>
#include <sstream>
#include "Zeal.h"
namespace Zeal
{
	namespace String
	{
		std::string replace(std::string input, const std::string& from, const std::string& to) {
			size_t start_pos = 0;
			while ((start_pos = input.find(from, start_pos)) != std::string::npos) {
				input.replace(start_pos, from.length(), to);
				start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
			}
			return input;
		}
		std::string trim_and_reduce_spaces(const std::string& input)
		{
			std::string result;
			bool inSpace = false;

			// First, remove leading spaces
			auto start = std::find_if_not(input.begin(), input.end(), [](unsigned char ch) { return std::isspace(ch); });

			// If the string is entirely spaces, return an empty string
			if (start == input.end()) {
				return "";
			}

			// Then, remove trailing spaces by finding the last non-space character
			auto end = std::find_if_not(input.rbegin(), input.rend(), [](unsigned char ch) { return std::isspace(ch); }).base();

			// Process the rest of the string
			for (auto it = start; it != end; ++it) {
				if (std::isspace(*it)) {
					if (!inSpace) {
						result += ' ';
						inSpace = true;
					}
				}
				else {
					result += *it;
					inSpace = false;
				}
			}

			return result;
		}

		bool compare_insensitive(const std::string& str1, const std::string& str2) {
			// Check if the strings are of different lengths, if so, they can't match
			if (str1.length() != str2.length()) {
				return false;
			}

			// Convert both strings to lowercase and then compare
			std::string str1Lower = str1;
			std::string str2Lower = str2;
			std::transform(str1Lower.begin(), str1Lower.end(), str1Lower.begin(), ::tolower);
			std::transform(str2Lower.begin(), str2Lower.end(), str2Lower.begin(), ::tolower);

			// Now, perform a case-insensitive comparison
			return str1Lower == str2Lower;
		}

		std::vector<std::string> split(const std::string& str, const std::string& delim) {
			std::string trimmed = trim_and_reduce_spaces(str);
			std::vector<std::string> tokens;
			size_t start = 0, end = 0;
			while ((end = trimmed.find(delim, start)) != std::string::npos) {
				tokens.push_back(trimmed.substr(start, end - start));
				start = end + delim.length();
			}
			tokens.push_back(trimmed.substr(start));
			return tokens;
		}


		bool tryParse(const std::string& str, int* result) {
			try {
				*result = std::stoi(str);
				return true;
			}
			catch (const std::invalid_argument& e) {
				Zeal::EqGame::print_chat("Invalid Argument %s", e.what());
				return false;
			}
			catch (const std::out_of_range& e) {
				Zeal::EqGame::print_chat("Out of range: %s", e.what());
				return false;
			}
		}

		bool tryParse(const std::string& str, float* result) {
			try {
				*result = std::stof(str);
				return true;
			}
			catch (const std::invalid_argument& e) {
				Zeal::EqGame::print_chat("Invalid Argument %s", e.what());
				return false;
			}
			catch (const std::out_of_range& e) {
				Zeal::EqGame::print_chat("Out of range: %s", e.what());
				return false;
			}
		}
		std::string bytes_to_hex(const char* byteArray, size_t length) {
			std::ostringstream oss;
			oss << std::hex << std::setfill('0');
			for (size_t i = 0; i < length; ++i) {
				oss << std::setw(2) << static_cast<unsigned int>(static_cast<unsigned char>(byteArray[i])) << " ";
			}
			return oss.str();
		}

	}
}