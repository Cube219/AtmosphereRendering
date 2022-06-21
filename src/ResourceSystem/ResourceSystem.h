#pragma once

#ifndef __RESOURCE_SYSTEM_H__
#define __RESOURCE_SYSTEM_H__

#include "BaseHeader.h"

class ResourceSystem
{
public:
    ResourceSystem() = delete;
    ~ResourceSystem() = delete;

    static void Initizlie();
    static void Shutdown();

    template <typename T>
    SPtr<T> LoadResource(const char* path)
    {
        return T::Load(path);
    }
};

#endif // __RESOURCE_SYSTEM_H__
