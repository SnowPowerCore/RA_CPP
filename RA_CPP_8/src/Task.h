#pragma once

#include <experimental/coroutine>

/**
 * @class Task
 * @brief Определяет поведение сопрограммы, выполняющую некоторые вычисления и возвращающую результат.
 * @tparam T тип возвращаемого значения
 */
template<typename T>
class Task final
{
public:
    struct promise_type;
    using coroutine_handle = std::experimental::coroutine_handle<promise_type>;

    struct promise_type final
    {
        T value; // NOTE: Храним значение, возвращаемое из сопрограммы.

        // NOTE: Получаем экземпляр Task для возврата из сопрограммы.
        Task get_return_object()
        {
            return coroutine_handle::from_promise(*this);
        }

        // NOTE: Что делать при первом заходе в сопрограмму.
        auto initial_suspend()
        {
            // NOTE: Не останавливать выполнение.
            return std::experimental::suspend_never();
        }

        // NOTE: Что делать при завершении сопрограммы.
        auto final_suspend()
        {
            // NOTE: Приостанавливаем сопрограмму при завершении.
            // WARNING: Необходимо в этом случае самостоятельно освобождать её ресурсы.
            return std::experimental::suspend_always();
        }

        // NOTE: Как обрабатывать выражение co_return ...
        void return_value(T&& other)
        {
            value = std::forward<T>(other);
        }

        // NOTE: Если есть необработанные исключения, аварийно завершаем программу.
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
        // NOTE: Освобождаем ресурсы сопрограммы.
        if (handle_) {
            handle_.destroy();
        }
    }

    /**
     * @brief Возобновляет выполнение сопрограммы.
     * @return значение, показывающее может ли сопрограмма быть возобновлена (ещё не завершена)
     */
    bool resume()
    {
        // NOTE: Если не ещё завершена, то возобновляем.
        if (!handle_.done()) {
            handle_.resume();
        }

        return !handle_.done();
    }

    /**
     * @brief Показывает, надо ли останавливать сопрограмму.
     */
    bool await_ready() const
    {
        return handle_.done();
    }

    /**
     * @brief Вызывается после остановки сопрограммы.
     */
    void await_suspend(std::experimental::coroutine_handle<>)
    {
        // NOTE: Ничего не делаем.
    }

    /**
     * @brief Вызывается при возобновлении сопрограммы.
     */
    auto await_resume()
    {
        // NOTE: Просто возвращаем значение.
        return handle_.promise().value;
    }

private:
    coroutine_handle handle_;
};

/**
 * @class Task<void>
 * @brief Специализация задачи для типа void. Выполнение некоторого кода без возврата какого-либо значения.
 */
template<>
class Task<void> final
{
public:
    struct promise_type;
    using coroutine_handle = std::experimental::coroutine_handle<promise_type>;

    struct promise_type final
    {
        Task get_return_object()
        {
            return coroutine_handle::from_promise(*this);
        }

        auto initial_suspend()
        {
            return std::experimental::suspend_never();
        }

        auto final_suspend()
        {
            return std::experimental::suspend_always();
        }

        // NOTE: Обрабатываем выход из сопрограммы без возвращаемого значения.
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
