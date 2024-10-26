#pragma once

namespace spry {

class Scene {
public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual void load();
    virtual void process(float delta);
    virtual void update(float delta);
    virtual void unload();

private:
};

}