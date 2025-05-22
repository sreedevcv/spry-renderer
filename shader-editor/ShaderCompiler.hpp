#pragma once

#include <expected>
#include <string>

class ShaderRaii {
public:
    ShaderRaii(int32_t id);
};

// class ShaderEditor {
// public:
std::expected<int32_t, std::string> compile(const char* shader);

// private:
// };