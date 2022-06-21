#include "GameObjectSystem.h"

#include "GameObject.h"

Vector<SPtr<GameObject>> GameObjectSystem::mGameObjects;
Vector<SPtr<GameObject>> GameObjectSystem::mGameObjectsToAdd;
Vector<SPtr<GameObject>> GameObjectSystem::mGameObjectsToRemove;

void GameObjectSystem::Initizlie()
{
}

void GameObjectSystem::Shutdown()
{
    mGameObjectsToRemove.clear();
    mGameObjectsToAdd.clear();
    mGameObjects.clear();
}

void GameObjectSystem::Update(double dt)
{
    mGameObjects.insert(mGameObjects.end(), mGameObjectsToAdd.begin(), mGameObjectsToAdd.end());
    for(auto toRemove : mGameObjectsToRemove) {
        auto findIt = std::find(mGameObjects.begin(), mGameObjects.end(), toRemove);
        if(findIt != mGameObjects.end()) {
            mGameObjects.erase(findIt);
        }
    }
    mGameObjectsToAdd.clear();
    mGameObjectsToRemove.clear();

    for(auto o : mGameObjects) {
        o->PreUpdate();
    }

    for(auto o : mGameObjects) {
        o->Update(dt);
    }

    mGameObjects.insert(mGameObjects.end(), mGameObjectsToAdd.begin(), mGameObjectsToAdd.end());
    mGameObjectsToAdd.clear();

    for(auto o : mGameObjects) {
        o->PostUpdate();
    }
}

void GameObjectSystem::RegisterGameObject(SPtr<GameObject> gameObject)
{
    mGameObjectsToAdd.push_back(gameObject);
}

void GameObjectSystem::UnregisterGameObject(SPtr<GameObject> gameObject)
{
    mGameObjectsToRemove.push_back(gameObject);
}

void GameObjectSystem::UnregisterGameObjectImmediate(SPtr<GameObject> gameObject)
{
    auto findIt = std::find(mGameObjects.begin(), mGameObjects.end(), gameObject);
    if(findIt != mGameObjects.end()) {
        mGameObjects.erase(findIt);
    }
}
