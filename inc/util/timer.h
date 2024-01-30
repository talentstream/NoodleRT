//
// Created by 44772 on 2024/1/30.
//

#pragma once

#include "core/common.h"

#include <chrono>
#include <print>

NAMESPACE_BEGIN

    class Timer {
    public:
        Timer() {
            Reset();
        }

        void Reset() {
            mStartTime = std::chrono::system_clock::now();
        }


        void PrintElapsedMs() const {
            auto now = std::chrono::system_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - mStartTime).count();
            std::print("Elapsed: {}ms\n", elapsed);
        }

        void PrintElapsedSec() const {
            auto now = std::chrono::system_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - mStartTime).count();
            std::print("Elapsed: {}s\n", elapsed);
        }

    private:
        std::chrono::system_clock::time_point mStartTime;
    };

NAMESPACE_END
