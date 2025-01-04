#include "Line.hpp"
#include "VAO.hpp"

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

spry::Line::Line(Line&& line)
{
    if (this != &line) {
        mStart = std::move(line.mStart);
        mEnd = std::move(line.mEnd);
        mVAO = std::move(line.mVAO);
    }
}

spry::Line& spry::Line::operator=(Line&& line)
{
    if (this != &line) {
        mStart = std::move(line.mStart);
        mEnd = std::move(line.mEnd);
        mVAO = std::move(line.mVAO);
    }
    return *this;
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
    mVAO.load(std::span { vertices }, std::span { format }, 2, GL_STATIC_DRAW);
}