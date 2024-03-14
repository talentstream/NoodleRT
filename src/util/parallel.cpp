//
// Created by 44772 on 2024/1/25.
//

#include "util/parallel.h"
#include "util/threadpool.h"

NAMESPACE_BEGIN

void Parallel::For1D(Int start, Int end, const std::function<void(Int)> &func) {
    static ThreadPool pool;

    for (Int i = start; i < end; ++i) {
        pool.AddTask(func, i);
    }

    pool.FinishAllTask();
}

void Parallel::For2D(Int startX, Int endX, Int startY, Int endY,
                     const std::function<void(Int, Int)> &func) {
    static ThreadPool pool;

    for (Int x = startX; x < endX; ++x) {
        for (Int y = startY; y < endY; ++y) {
            pool.AddTask(func, x, y);
        }
    }
    pool.FinishAllTask();
}

NAMESPACE_END