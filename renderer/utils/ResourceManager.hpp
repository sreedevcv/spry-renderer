#pragma once

#include "spdlog/spdlog.h"
#include <unordered_map>
#include <string>
#include <string_view>
#include <utility>

namespace spry {

template <typename Res>
class ResourceManager {
public:
    Res& get(std::string_view name)
    {
        return mResources.at(std::string(name));
    }

    void insert(const std::string_view name, Res&& resource)
    {
        spdlog::info("Inserted name into ResourceManager");
        std::pair<std::string, Res> p = { std::string(name), std::move(resource) };
        mResources.insert(std::move(p));
    }
    std::unordered_map<std::string, Res> mResources;
    ResourceManager() = default;
};

}
