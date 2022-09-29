#pragma once
#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdio.h>
#include <string.h>
#include <type_traits>

extern char msgBuf[10001]; // Defined in Logger.cpp

enum class LogType
{
    Info, Warning, Error
};

const char* SplitFileName(const char* fullPath);

template <typename ...Args>
void PrintLog(LogType type, const char* fileName, int lineNum, const char* msg, Args&&... args)
{
    // INFO [Core.cpp:152]: ASDF
    const char* prefix = "";
    switch(type) {
        case LogType::Info:
            prefix = " INFO";
            break;
        case LogType::Warning:
            prefix = " WARN";
            break;
        case LogType::Error:
            prefix = "ERROR";
            break;
    }

    sprintf_s(msgBuf, msg, std::forward<Args>(args)...);

    printf("%s [%s:%d]: %s\n", prefix, SplitFileName(fileName), lineNum, msgBuf);
}

#define LOG(msg, ...) PrintLog(LogType::Info, __FILE__, __LINE__, msg, ##__VA_ARGS__)
#define LOG_WARNING(msg, ...) PrintLog(LogType::Warning, __FILE__, __LINE__, msg, ##__VA_ARGS__)
#define LOG_ERROR(msg, ...) PrintLog(LogType::Error, __FILE__, __LINE__, msg, ##__VA_ARGS__)

#endif // __LOGGER_H__
