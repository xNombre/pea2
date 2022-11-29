#include "TimeBench.hpp"

#include <pthread.h>
#include <iostream>
template <typename ReturnType>
TimeBench<ReturnType>::TimeBench(task_function_t fun)
    : task_function(fun)
{
}

template <typename ReturnType>
TimeBench<ReturnType>::~TimeBench()
{
    if (task_thread.joinable())
        task_thread.join();
    watchdog_thread.join();
}

template <typename ReturnType>
auto TimeBench<ReturnType>::start_benchmark(const timeout_t &timeout) -> std::future<result_t>
{
    if (task_thread.joinable() || watchdog_thread.joinable()) {
        throw std::runtime_error("Benchmark task is already running!");
    }

    auto future = task_promise.get_future();

    std::unique_lock lock(task_result_mutex);
    watchdog_thread = std::thread([&](const timeout_t &timeout) {
        watchdog_function(timeout);
                                  }, timeout);
    task_thread = std::thread([&] {
        int old;
        // Change cancel type to be able to kill the thread immediately
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old);
        benchmark_function();
                              });
    native_handle = task_thread.native_handle();

    return std::move(future);
}

template <typename ReturnType>
void TimeBench<ReturnType>::benchmark_function()
{
    auto time_start = std::chrono::system_clock::now();
    auto result = task_function();
    auto time_end = std::chrono::system_clock::now();

    // Prepare result and wake up watchdog to commit future data
    std::unique_lock lock(task_result_mutex);

    task_result.result = result;
    task_result.measured_time = time_end - time_start;
    task_result.task_finished = true;

    lock.unlock();
    watchdog_timeout.notify_one();
}

template <typename ReturnType>
void TimeBench<ReturnType>::watchdog_function(const timeout_t &timeout)
{
    std::unique_lock lock(task_result_mutex);
    // Use wait_until to avoid spurious wakeups
    if (task_result.task_finished) {
        task_promise.set_value(std::move(task_result));
        return;
    }
    auto status = watchdog_timeout.wait_until(lock, std::chrono::system_clock::now() + timeout);

    if (status == std::cv_status::timeout) {
        task_thread.detach();
        pthread_cancel(native_handle);

        task_result.task_finished = false;
    }

    task_promise.set_value(std::move(task_result));
}