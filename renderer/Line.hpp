#pragma once

#include <glm/vec3.hpp>

#include "VAO.hpp"

namespace spry {

class Line {
private:
    glm::vec3 mStart;
    glm::vec3 mEnd;

    VAO mVAO;

    void loadMesh();

public:
    Line();
    Line(glm::vec3 start, glm::vec3 end );
    ~Line() = default;

    void setEndPoints(glm::vec3 start, glm::vec3 end);
    void draw();
    void unload();
};

}