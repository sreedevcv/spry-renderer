#pragma once

namespace spry {

class Model {
public:
    Model() = default;
    ~Model() = default;

    void load(const char* path);
private:
};

}