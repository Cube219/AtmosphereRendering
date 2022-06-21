#include "Logger.h"

char msgBuf[101];

const char* SplitFileName(const char* fullPath)
{
    const char* pitch;

#ifdef _WIN32
    pitch = strrchr(fullPath, '\\');
#else
    pitch = fullPath;
#endif

    if(pitch != nullptr)
        return pitch + 1;

    return fullPath;
}
