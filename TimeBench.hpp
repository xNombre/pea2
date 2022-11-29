#pragma once

#include <functional>
#include <future>
#include <chrono>
#include <mutex>
#include <condition_variable>

template<typename ReturnType>
struct FutureResult {
    ReturnType result;
    bool task_finished = false;
    std::chrono::duration<double> measured_time;
};

template<typename ReturnType>
class TimeBench {
    typedef std::function<ReturnType(void)> task_function_t;
    typedef FutureResult<ReturnType> result_t;
    typedef std::chrono::duration<double> timeout_t;

public:
    TimeBench(task_function_t fun);
    ~TimeBench();
    std::future<result_t> start_benchmark(const timeout_t &timeout);

private:
    void benchmark_function();
    void watchdog_function(const timeout_t &timeout);

    FutureResult<ReturnType> task_result;
    std::promise<result_t> task_promise;
    std::thread task_thread, watchdog_thread;
    std::thread::native_handle_type native_handle;
    std::mutex task_result_mutex;
    task_function_t task_function;
    std::condition_variable watchdog_timeout;
};
