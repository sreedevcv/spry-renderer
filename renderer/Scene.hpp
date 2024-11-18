#pragma once

namespace spry {

class Scene {
public:
    Scene() = default;
    virtual ~Scene() = default;

    Scene(const Scene& scene) = delete;
    Scene& operator=(const Scene& scene) = delete;

    /* process and update are called each frame but no garuntee
     * that process and update would be called one after the another
     */

    virtual void load();
    virtual void process(float delta);
    virtual void draw();
    virtual void unload();

private:
};

}