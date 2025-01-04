#pragma once

#include <glm/vec3.hpp>

#include "VAO.hpp"

namespace spry {

class Line {
public:
    Line();
    Line(glm::vec3 start, glm::vec3 end);
    ~Line() = default;

    Line(Line&& line);
    Line& operator=(Line&& line);
    Line(const Line& line) = delete;
    Line& operator=(const Line& line) = delete;

    void setEndPoints(glm::vec3 start, glm::vec3 end);
    void draw() const;
    
private:
    glm::vec3 mStart;
    glm::vec3 mEnd;
    VAO mVAO;

    void loadMesh();
    void unload() const;
};

}