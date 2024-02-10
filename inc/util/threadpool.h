#pragma once

#include "core/common.h"
#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable>
#include <functional>
#include <queue>
#include <memory>
#include <future>

NAMESPACE_BEGIN

namespace detail {
#ifdef __cpp_lib_move_only_function
using default_function_type = std::move_only_function<void()>;
#else
using default_function_type = std::function<void()>;
#endif
}

template<typename FunctionType = detail::default_function_type,
        typename ThreadType = std::thread>

class ThreadPool {
public:
    NON_COPY(ThreadPool)

    ThreadPool(const Integer threadCount = std::thread::hardware_concurrency()) {
        mPool = std::make_shared<Pool>();
        mThreads.reserve(threadCount);
        for (const auto i: std::views::iota(0, threadCount)) {
            mThreads.emplace_back(&ThreadPool::ProcessThread, this);
        }
    }

    ~ThreadPool() {
        FinishAllTask();
    }

    void FinishAllTask() {
        {
            std::unique_lock locker{mPool->mutex};
            mPool->exit = true;
        }

        mPool->condVar.notify_all();

        for (auto &thread: mThreads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }

    void ProcessThread() {

        while (true) {
            std::unique_lock locker{mPool->mutex};
            mPool->condVar.wait(locker, [this] { return !mPool->tasks.empty() || mPool->exit; });
            if (mPool->exit) {
                return;
            }
            auto task = std::move(mPool->tasks.front());
            mPool->tasks.pop();
            locker.unlock();
            task();
            locker.lock();
        }
    }

    template<typename Func, typename...Args>
    void AddTask(Func &&func, Args &&...args) {
        auto task = std::make_shared<std::packaged_task<decltype(func(args...))()>>(
                std::bind(std::forward<Func>(func),
                          std::forward<Args>(args)...));

        mThreads.emplace_back([task]() {
            (*task)();
        });
        mPool->condVar.notify_one();
    }

private:
    struct Pool {
        std::mutex mutex;
        std::condition_variable condVar;
        std::queue<FunctionType> tasks;
        Boolean exit{false};
    };
    std::shared_ptr<Pool> mPool;
    std::vector<ThreadType> mThreads;

};

NAMESPACE_END