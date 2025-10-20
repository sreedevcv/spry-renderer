#pragma once

#include <expected>
#include <string>

std::expected<int32_t, std::string> compile(const char* shader);