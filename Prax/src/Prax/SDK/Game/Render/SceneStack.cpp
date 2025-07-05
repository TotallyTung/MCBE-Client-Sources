//
// Created by vastrakai on 1/13/2024.
//

#include "SceneStack.h"

AbstractScene* SceneStack::GetScene(int index)
{
    if (index >= scenes.size()) return nullptr;
    return scenes[index].get();
}
