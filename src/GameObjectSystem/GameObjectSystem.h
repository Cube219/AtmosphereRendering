#pragma once

#ifndef __GAMEOBJECT_SYSTEM_H__
#define __GAMEOBJECT_SYSTEM_H__

#include "BaseHeader.h"

class GameObject;

class GameObjectSystem
{
public:
    GameObjectSystem() = delete;
    ~GameObjectSystem() = delete;

    static void Initizlie();
    static void Shutdown();

    static void Update(double dt);

    static void RegisterGameObject(SPtr<GameObject> gameObject);
    static void UnregisterGameObject(SPtr<GameObject> gameObject);
    static void UnregisterGameObjectImmediate(SPtr<GameObject> gameObject);

private:
    static Vector<SPtr<GameObject>> mGameObjects;
    static Vector<SPtr<GameObject>> mGameObjectsToAdd;
    static Vector<SPtr<GameObject>> mGameObjectsToRemove;
};

#endif // __GAMEOBJECT_SYSTEM_H__
