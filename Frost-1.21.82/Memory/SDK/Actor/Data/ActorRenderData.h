#pragma once

struct ActorRenderData {
    class Actor* actor;
    int64_t* data; // e. g. ItemStack if the actor is an ItemActor
    Vector3<float> position;
    Vector2<float> rotation;
    Vector2<float> headRot;
    bool glint;
    bool ignoreLighting;
    bool isInUI;
    float deltaTime;
    int modelObjId;
    float modelSize;
    //AnimationComponent *animationComponent;
    //MolangVariableMap *variables;

private:
    char pad[0x10];
};
