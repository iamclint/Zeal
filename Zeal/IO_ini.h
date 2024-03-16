#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <stdexcept>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <stdexcept>
#include "EqFunctions.h"
class IO_ini {
private:
    std::string filename;
public:
    IO_ini(const std::string& filename) : filename(filename) {   };

    void set(std::string path)
    {
        filename = path;
    }

    bool exists(const std::string& section, const std::string& key) const {
        char buffer[256];
        DWORD bytesRead = GetPrivateProfileStringA(section.c_str(), key.c_str(), "", buffer, sizeof(buffer), filename.c_str());

        if (bytesRead == 0) {
            return false;
        }
        return true;
    }

    std::vector<std::string> getSectionNames() {
        std::vector<std::string> sectionNames;
        const DWORD bufferSize = 4096; // Adjust buffer size as needed
        char buffer[bufferSize];

        DWORD result = GetPrivateProfileSectionNamesA(buffer, bufferSize, filename.c_str());
        if (result == 0) {
            std::cerr << "Failed to read INI file: " << filename << std::endl;
            return sectionNames;
        }

        for (char* p = buffer; *p != '\0'; p += strlen(p) + 1) {
            sectionNames.push_back(p);
        }

        return sectionNames;
    }

    bool deleteSection(const std::string& sectionName) {
        // Delete the section and its contents by writing an empty string to it
        if (!WritePrivateProfileSectionA(sectionName.c_str(), nullptr, filename.c_str())) 
        {
            return false;
        }
        else 
        {
            return true;
        }
    }

    template<typename T>
    T getValue(std::string section, std::string key) const {
        char buffer[256];
        DWORD bytesRead = GetPrivateProfileStringA(section.c_str(), key.c_str(), "", buffer, sizeof(buffer), filename.c_str());

        if (bytesRead == 0) {
            return T{};
        }

        return convertFromString<T>(std::string(buffer));
    }

    template<typename T>
    void setValue(const std::string& section, const std::string& key, const T& value) {

        std::string valueStr = std::to_string(value);
         if constexpr (std::is_same_v<T, bool>) {
             if (value)
                 valueStr = "TRUE";
            else
                 valueStr = "FALSE";
        }
        BOOL result = WritePrivateProfileStringA(section.c_str(), key.c_str(), valueStr.c_str(), filename.c_str());
        if (!result) {
            Zeal::EqGame::print_chat("Error writing value to INI file.");
        }
    }
private:
    template<typename T>
    T convertFromString(const std::string& str) const {

        if constexpr (std::is_same_v<T, bool>) {
            if (str == "TRUE")
                return true;
            else
                return false;
        }
        std::istringstream iss(str);
        T value;
        iss >> std::boolalpha >> value;
        return value;
    }
};
