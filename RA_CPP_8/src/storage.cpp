#include <atomic>
#include <functional>
#include <iostream>
#include <optional>
#include <shared_mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

using namespace::std::chrono_literals;

#define REQUIRES(...) typename = std::enable_if_t<__VA_ARGS__>

class SpinLock final
{
public:
    constexpr SpinLock() noexcept
        : flag_ ATOMIC_FLAG_INIT
    {}

    constexpr void lock() noexcept
    {
        while(flag_.test_and_set(std::memory_order_acquire));
    }

    constexpr void unlock() noexcept
    {
        flag_.clear(std::memory_order_release);
    }

private:
    std::atomic_flag flag_;
};

struct IStorage
{
    virtual ~IStorage() = default;
    virtual std::optional<std::string> get(int key) const = 0;
    virtual void set(int key, std::string_view value) = 0;
};

class MemoryStorage : public IStorage
{
public:
    std::optional<std::string> get(int key) const override
    {
        const auto it = map_.find(key);
        return (it == map_.cend()) ? std::nullopt : std::make_optional(it->second);
    }

    void set(int key, std::string_view value) override
    {
        map_[key] = value;
    }

private:
    std::unordered_map<int, std::string> map_;
};

template<typename Storage, REQUIRES(std::is_base_of_v<IStorage, Storage>)>
class ThreadSafeStorage : public Storage
{
public:
    std::optional<std::string> get(int key) const override
    {
        std::shared_lock lock(mutex_);
        std::this_thread::sleep_for(1s);
        return Storage::get(key);
    }

    void set(int key, std::string_view value) override
    {
        std::lock_guard lock(mutex_);
        std::this_thread::sleep_for(300ms);
        Storage::set(key, value);
    }

private:
    mutable std::shared_mutex mutex_;
};

namespace
{
    template<typename T>
    std::ostream& operator<<(std::ostream& out, const std::optional<T>& optional)
    {
        if (optional.has_value()) {
            out << optional.value();
        } else {
            out << "[ NONE ]";
        }

        return out;
    }
}


int main()
{
    ThreadSafeStorage<MemoryStorage> storage;
    storage.set(42, "text");

    std::thread([&storage] {
        storage.set(1, "one");
        storage.set(2, "two");
        storage.set(3, "three");
    }).join();

    std::vector<std::thread> threads;

    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([i, &storage] {
            std::cout << storage.get(i) << "\n";
        });
    }

    std::cout << storage.get(42) << "\n";

    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

    return 0;
}
