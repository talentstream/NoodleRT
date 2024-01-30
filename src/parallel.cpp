//
// Created by 44772 on 2024/1/25.
//

#include "util/parallel.h"
#include "util/threadpool.h"

NAMESPACE_BEGIN

void Parallel::For1D(Integer start, Integer end, const std::function<void(Integer)> &func) {
    static ThreadPool pool;

    for (Integer i = start; i < end; ++i) {
        pool.AddTask(func, i);
    }

    pool.FinishAllTask();
}

void Parallel::For2D(Integer startX, Integer endX, Integer startY, Integer endY,
                     const std::function<void(Integer, Integer)> &func) {
    static ThreadPool pool;

    for (Integer x = startX; x < endX; ++x) {
        for (Integer y = startY; y < endY; ++y) {
            pool.AddTask(func, x, y);
        }
    }
    pool.FinishAllTask();
}

NAMESPACE_END