#include "FontManager.hpp"
#include "Font.hpp"
#include "spdlog/spdlog.h"
#include <string>
#include <string_view>
#include <utility>

spry::Font& spry::FontManager::load(const char* fontName)
{
    if (mResources.contains(fontName)) {
        // spdlog::warn("Multiple loading of font: {}", fontName);
    } else {
        Font font(fontName, 30);
        insert(std::string_view(fontName), std::move(font));
    }

    return get(fontName);
}

spry::FontManager& spry::FontManager::instance()
{
    static FontManager fontManager;
    return fontManager;
}