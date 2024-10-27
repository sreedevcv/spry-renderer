#include "Line.hpp"

#include <vector>

spry::Line::Line()
    : mStart(glm::vec3(0.0f, 0.0f, 0.0f))
    , mEnd(glm::vec3(1.0f, 1.0f, 1.0f))
{
    loadMesh();
}

spry::Line::Line(glm::vec3 start, glm::vec3 end)
    : mStart(start)
    , mEnd(end)
{
    loadMesh();
}

void spry::Line::setEndPoints(glm::vec3 start, glm::vec3 end)
{
    mStart = start;
    mEnd = end;
    loadMesh();
}

void spry::Line::draw() const
{
    mVAO.draw(GL_LINES);
}

void spry::Line::loadMesh()
{
    std::vector<float> vertices = {
        mStart.x, mStart.y, mStart.z, //
        mEnd.x, mEnd.y, mEnd.z, //
    };

    std::vector<uint32_t> format = { 3 };
    mVAO.load(std::span { vertices }, std::span { format });
}

void spry::Line::unload() const
{
    mVAO.unload();
}