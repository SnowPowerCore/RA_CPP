#pragma once

#include <type_traits>

#include <experimental/coroutine>

#define REQUIRES(...) typename = std::enable_if_t<__VA_ARGS__>

template<typename T = int, REQUIRES(std::is_arithmetic_v<T>)>
class Generator final
{
public:
    struct promise_type;
    using coroutine_handle = std::experimental::coroutine_handle<promise_type>;

    struct promise_type final {
        T value;

        Generator get_return_object() {
            return coroutine_handle::from_promise(*this);
        }

        auto initial_suspend() {
            return std::experimental::suspend_never();
        }

        auto final_suspend() {
            return std::experimental::suspend_always();
        }

        void return_void() {}

        auto yield_value(T other) {
            value = other;
            return std::experimental::suspend_always();
        }

        [[noreturn]] void unhandled_exception()
        {
            std::terminate();
        }
    };

    struct Iterator final
    {
        using iterator_category = std::input_iterator_tag;
        using value_type = T;

        coroutine_handle handle;

        Iterator(coroutine_handle handle)
            : handle(handle)
        {}

        Iterator& operator++() {
            handle.resume();

            if (handle.done()) {
                handle = nullptr;
            }

            return *this;
        }

        bool operator==(const Iterator& other) const {
            return handle == other.handle;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }

        const T& operator*() const {
            return handle.promise().value;
        }
    };

public:
    Generator() = default;

    Generator(T first, T last)
        : first_(first)
        , last_(last)
        , handle_(nullptr)
    {}

    Generator(coroutine_handle handle)
        : handle_(handle)
    {}

    Generator(const Generator& other) = delete;
    Generator(Generator&& other) noexcept = default;

    ~Generator()
    {
        if (handle_) {
            handle_.destroy();
        }
    }

    Generator& operator=(const Generator& other) = delete;
    Generator& operator=(Generator&& other) noexcept = default;

    Iterator begin()
    {
        if (!handle_) {
            return end();
        }

        handle_.resume();

        return handle_.done() ? nullptr : handle_;
    }

    Iterator end()
    {
        return Iterator(nullptr);
    }

private:
    T first_;
    T last_;
    coroutine_handle handle_;
};
