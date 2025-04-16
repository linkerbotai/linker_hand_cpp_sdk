#ifndef YAML_CONFIG_H
#define YAML_CONFIG_H

#include <yaml-cpp/yaml.h>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <mutex>


class YamlConfig {
public:
    static YamlConfig& getInstance() {
        static YamlConfig instance;
        return instance;
    }

    void loadConfig(const std::string& filename) {
        std::lock_guard<std::mutex> lock(mutex_);
        try {
            config = YAML::LoadFile(filename);
        } catch (const YAML::Exception& e) {
            throw std::runtime_error("Failed to load YAML file: " + std::string(e.what()));
        }
        std::cout << "Config loaded successfully" << std::endl;
    }

    std::string getString(const std::string& key) const {
        return config[key].as<std::string>();
    }

    int getInt(const std::string& key) const {
        return config[key].as<int>();
    }

    double getDouble(const std::string& key) const {
        return config[key].as<double>();
    }

    std::string getNestedString(const std::vector<std::string>& keys) const {
        YAML::Node node = config;
        for (const auto& key : keys) {
            node = node[key];
        }
        return node.as<std::string>();
    }

    std::vector<std::string> getStringList(const std::string& key) const {
        std::vector<std::string> result;
        const YAML::Node& list = config[key];
        for (const auto& item : list) {
            result.push_back(item.as<std::string>());
        }
        return result;
    }

private:
    YamlConfig() = default;
    YamlConfig(const YamlConfig&) = delete;
    YamlConfig& operator=(const YamlConfig&) = delete;

    YAML::Node config;
    mutable std::mutex mutex_;
};

#endif // YAML_CONFIG_H