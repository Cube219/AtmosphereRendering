#include "Utility.h"

std::random_device Random::randomDevice;
std::mt19937 Random::randomEngine;

void Random::Initizlie()
{
    randomEngine = std::mt19937(randomDevice());
}

void Random::Shutdown()
{
}
