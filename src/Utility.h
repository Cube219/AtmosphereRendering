#pragma once

#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "BaseHeader.h"

#include <random>

class Random
{
public:
    Random() = delete;
    ~Random() = delete;

    static void Initizlie();
    static void Shutdown();

    static int Range(int min, int max)
    {
        std::uniform_int_distribution<int> dis(min, max);
        return dis(randomEngine);
    }
    static float Range(float min, float max)
    {
        std::uniform_real_distribution<float> dis(min, max);
        return dis(randomEngine);
    }

private:
    static std::random_device randomDevice;
    static std::mt19937 randomEngine;
};

constexpr double PI = 3.141592653;

#endif // __UTILITY_H__
