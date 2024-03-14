//
// Created by 44772 on 2024/1/30.
//

#pragma once

#define NAMESPACE_BEGIN namespace nrt {
#define NAMESPACE_END }

using Float = float;
using Int = int;
using Bool = bool;
using UInt8 = unsigned char;
using UInt32 = unsigned int;

#define NON_COPY(ClassName) \
    ClassName(const ClassName&) = delete; \
    ClassName& operator=(const ClassName&) = delete; \
    ClassName(ClassName&&) = delete; \
    ClassName& operator=(ClassName&&) = delete;