#include <iostream>
#include <mutex>
#include <string>
#include <unordered_map>

#define REQUIRES(...) typename = std::enable_if_t<__VA_ARGS__>

struct IStorage {
    virtual ~IStorage() = default;
    virtual std::string get(int key) const = 0;
    virtual void set(int key, const std::string& value) = 0;
};

class MemoryStorage : public IStorage
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

// NOTE: Реализуем потоко-безопасное хранилище путём наследования от шаблонного параметра.
// Сравните с примером из 2-го занятия.
template<typename Storage, REQUIRES(std::is_base_of_v<IStorage, Storage>)>
class ThreadSafeStorage : public Storage
{
public:
    void set(int key, const std::string& value) override
    {
        static std::mutex mutex;
        std::lock_guard lock(mutex);

        Storage::set(key, value);
    }
};

int main()
{
   ThreadSafeStorage<MemoryStorage> storage;
   storage.set(42, "text");
   std::cout << storage.get(42) << "\n";

    return 0;
}
