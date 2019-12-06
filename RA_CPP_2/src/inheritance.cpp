#include <cstring>
#include <iostream>
#include <mutex>
#include <string>
#include <unordered_map>

class ByteArray final
{
public:
    explicit ByteArray(size_t size)
        : size_(size)
        , data_(new std::byte[size_])
    {}

    ~ByteArray()
    {
        delete[] data_;
    }

private:
    size_t size_;
    std::byte* data_;
};

// NOTE: Инкапсулируем выделение/освобождение памяти в аллокаторе.
struct Allocator
{
    [[nodiscard]] std::byte* allocate(size_t size)
    {
        return new std::byte[size];
    }

    void deallocate(std::byte* data, size_t)
    {
        delete[] data;
    }
};

class ByteArray2 final
{
public:
    explicit ByteArray2(size_t size)
        : size_(size)
        , data_(allocator_.allocate(size))
    {}

    ~ByteArray2()
    {
        allocator_.deallocate(data_, size_);
    }

private:
    Allocator allocator_; // NOTE: Композиция посредством добавления поля для аллокатора.
    size_t size_;
    std::byte* data_;
};

class ByteArray3 : private Allocator // NOTE: Композиция посредством приватного наследования аллокатора.
{
public:
    explicit ByteArray3(size_t size)
        : size_(size)
        , data_(allocate(size))
    {}

    ~ByteArray3()
    {
        deallocate(data_, size_);
    }

private:
    size_t size_;
    std::byte* data_;
};

struct IStorage
{
    virtual ~IStorage() = default;
    virtual std::string get(int key) const = 0;
    virtual void set(int key, const std::string& value) = 0;
};

class MemoryStorage : virtual public IStorage
{
public:
    std::string get(int key) const override
    {
        const auto it = map_.find(key);
        return (it == map_.cend()) ? "" : it->second;
    }

    void set(int key, const std::string& value) override
    {
        map_[key] = value;
    }

private:
    std::unordered_map<int, std::string> map_;
};

// NOTE: Реализуем потоко-безопасное хранилище посредством приватного и виртуального наследования.
// WARNING: Только для демострации. В реальном коде так делать нельзя.
class ThreadSafeStorage : virtual public IStorage, private MemoryStorage
{
public:
    // NOTE: Предполагая, что метод "get()" не требует защиты мьютексом, просто "пробрасываем" от базового класса.
    using MemoryStorage::get;

    void set(int key, const std::string& value) override
    {
        static std::mutex mutex;
        std::lock_guard lock(mutex);

        MemoryStorage::set(key, value);
    }
};

int main()
{
    {
        std::cout << sizeof(ByteArray) << "\n";
        std::cout << sizeof(Allocator) << "\n\n";

        std::cout << sizeof(ByteArray2) << "\n";
        std::cout << sizeof(ByteArray3) << "\n\n";
    }

    {
        ThreadSafeStorage storage;
        storage.set(42, "text");
        std::cout << storage.get(42) << "\n";
    }

    return 0;
}
