#pragma once

#include <experimental/coroutine>

template<typename T>
class Task final
{
public:
    struct promise_type;
    using coroutine_handle = std::experimental::coroutine_handle<promise_type>;

    struct promise_type final {
        T value;

        Task get_return_object() {
            return coroutine_handle::from_promise(*this);
        }

        auto initial_suspend() {
            return std::experimental::suspend_always();
        }

        auto final_suspend() {
            return std::experimental::suspend_always();
        }

        void return_value(T&& other) {
            value = std::forward<T>(other);
        }

        [[noreturn]] void unhandled_exception()
        {
            std::terminate();
        }
    };

public:
    Task(coroutine_handle handle)
        : handle_(handle)
    {}

    Task(const Task& other) = delete;
    Task(Task&& other) noexcept = default;

    ~Task()
    {
        if (handle_) {
            handle_.destroy();
        }
    }

    bool resume()
    {
        if (!handle_.done()) {
            handle_.resume();
        }

        return !handle_.done();
    }

    bool await_ready() const {
        return handle_.done();
    }

    void await_suspend(std::experimental::coroutine_handle<>) {
        return handle_.resume();
    }

    auto await_resume() {
        return handle_.promise().value;
    }

private:
    coroutine_handle handle_;
};

template<>
class Task<void> final
{
public:
    struct promise_type;
    using coroutine_handle = std::experimental::coroutine_handle<promise_type>;

    struct promise_type final {
        Task get_return_object() {
            return coroutine_handle::from_promise(*this);
        }

        auto initial_suspend() {
            return std::experimental::suspend_always();
        }

        auto final_suspend() {
            return std::experimental::suspend_always();
        }

        void return_void() {}

        [[noreturn]] void unhandled_exception()
        {
            std::terminate();
        }
    };

public:
    Task(coroutine_handle handle)
        : handle_(handle)
    {}

    Task(const Task& other) = delete;
    Task(Task&& other) noexcept = default;

    ~Task()
    {
        if (handle_) {
            handle_.destroy();
        }
    }

    bool resume()
    {
        if (!handle_.done()) {
            handle_.resume();
        }

        return !handle_.done();
    }

private:
    coroutine_handle handle_;
};
