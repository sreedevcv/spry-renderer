#pragma once

#include "Font.hpp"
#include "ResourceManager.hpp"

namespace spry {

class FontManager : public ResourceManager<Font> {
public:
    Font& load(const char* fontName);
    static FontManager& instance();
private:
};

}