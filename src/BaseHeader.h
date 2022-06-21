#pragma once

#ifndef __BASE_HEADER_H__
#define __BASE_HEADER_H__

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
using namespace glm;

using Int8 = int8_t;
using Int16 = int16_t;
using Int32 = int32_t;
using Int64 = int64_t;

using Uint8 = uint8_t;
using Uint16 = uint16_t;
using Uint32 = uint32_t;
using Uint64 = uint64_t;

#include <vector>
template <typename T>
using Vector = std::vector<T>;

#include <map>
template <typename Key, typename Value>
using Map = std::map<Key, Value>;

#include <unordered_map>
template <typename Key, typename Value>
using HashMap = std::unordered_map<Key, Value>;

#include <memory>
template <typename T>
using SPtr = std::shared_ptr<T>;

#include <string>
using String = std::string;

#include "Logger.h"

#endif // __BASE_HEADER_H__
