#pragma once
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <mutex>
#include <Zeal.h>

namespace Zeal
{
	namespace Settings
	{
        class SettingsManager
        {
        private:
            std::unordered_map<std::string, std::any> settings;
            std::mutex mtx; // For thread safety

        public:
            // Set a setting by name and value
            template <typename T>
            void Set(const std::string& name, const T& value)
            {
                std::lock_guard<std::mutex> lock(mtx);
                settings[name] = value;
            }

            // Get a setting by name with an optional default value
            template <typename T>
            T Get(const std::string& name, const T& defaultValue = T()) const
            {
                std::lock_guard<std::mutex> lock(mtx);
                auto it = settings.find(name);
                if (it != settings.end())
                {
                    // Try to retrieve the value as the correct type
                    try
                    {
                        return std::any_cast<T>(it->second);
                    }
                    catch (const std::bad_any_cast&)
                    {
                        throw std::invalid_argument("Failed to cast the setting to the requested type.");
                    }
                }
                return defaultValue; // Return the default if not found
            }
        };

        // Singleton access:
        static SettingsManager settingsManager;

        // Wrapper functions:
        template <typename T>
        void Set(const std::string& name, const T& value)
        {
            settingsManager.Set(name, value);
        }

        template <typename T>
        T Get(const std::string& name, const T& defaultValue = T())
        {
            return settingsManager.Get(name, defaultValue);
        }
	}
}
