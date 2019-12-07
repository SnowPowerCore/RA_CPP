#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

#define REQUIRES(...) typename = std::enable_if_t<__VA_ARGS__>

using namespace std::chrono_literals;

/**
 * @class DebugMutex
 * @brief Отладочная обёртка над мьютексом. Выводит в stdout информации о блокировках.
 */
template<typename Mutex>
class DebugMutex final : private Mutex
{
public:
    explicit DebugMutex(std::string_view label)
        : label_(label)
    {}

    void lock()
    {
        Mutex::lock();
        std::cout << label_ << " locked from " << std::this_thread::get_id() << std::endl;
    }

    void unlock()
    {
        Mutex::unlock();
        std::cout << label_ << " unlocked from " << std::this_thread::get_id() << std::endl;
    }

private:
    std::string label_;
};

/**
 * @class BTree
 * @brief Бинарное дерево поиска.
 * @warning Балансировки нет. Не используйте в реальном коде.
 * @tparam Сompare функция сравнения ключей
 */
template<typename T, typename Compare = std::less<T>>
struct BTree final
{
    T value;
    std::unique_ptr<BTree> left;
    std::unique_ptr<BTree> right;

    explicit BTree(T&& value)
        : value(std::forward<T>(value))
    {}

    void insert(T&& value)
    {
        // NOTE: Защищаем вставку в дерево в мьютексом.
        static DebugMutex<std::mutex> mutex("InsertMutex");
        std::lock_guard lock(mutex);

        std::this_thread::sleep_for(1ms);

        auto& tree = Compare()(value, this->value) ? left : right;

        if (tree) {
           // NOTE: Из-за рекурсии возникнет взаимная блокировка.
            tree->insert(std::forward<T>(value));
        } else {
            tree = std::make_unique<BTree>(std::forward<T>(value));
        }
    }
};

class User
{
public:
    std::string login() const
    {
        return login_;
    }

    void setLogin(std::string_view login)
    {
        login_ = login;
    }

    std::string password() const
    {
        return password_;
    }

    void setPassword(std::string_view password)
    {
        password_ = password;
    }

private:
    std::string login_;
    std::string password_;
};

// NOTE: Свяжем с каждым экземпляром класса User пару мьютексов (по одному на каждое из полей).
struct ThreadSafeUser : public User
{
    ThreadSafeUser()
        : loginMutex("LoginMutex")
        , passwordMutex("PasswordMutex")
    {}

    DebugMutex<std::mutex> loginMutex;
    DebugMutex<std::mutex> passwordMutex;
};

namespace
{
    template<typename BTree, typename Callable, REQUIRES(std::is_invocable_v<Callable, BTree>)>
    void traverse(BTree tree, Callable&& visit)
    {
        if (tree) {
            visit(tree);
            traverse(tree->left.get(), visit);
            traverse(tree->right.get(), visit);
        }
    }

    std::ostream& operator<<(std::ostream& out, const User& user)
    {
        return out << "User(" << user.login() << ", " << user.password() << ")";
    }
}

int main()
{
    {
        BTree tree(42);

        std::thread first([&tree] { tree.insert(10); });
        std::thread second([&tree] { tree.insert(100); });
        std::thread third([&tree] { tree.insert(19); });

        first.join();
        second.join();
        third.join();

        traverse(&tree, [](auto* tree) { std::cout << tree->value << "\n"; });
    }

    // NOTE: Захватываем мьютексы в разном порядке и получаем взаимную блокировку.
    {
        ThreadSafeUser user;

        std::thread first([&user] {
            std::lock_guard loginLock(user.loginMutex);
            user.setLogin("alice");

            std::this_thread::sleep_for(1ms);

            std::lock_guard passwordLock(user.passwordMutex);
            user.setPassword("alice_123");
        });

        std::thread second([&user] {
            std::lock_guard passwordLock(user.passwordMutex);
            user.setPassword("qwerty");

            std::this_thread::sleep_for(1ms);

            std::lock_guard loginLock(user.loginMutex);
            user.setLogin("bob");
        });

        first.join();
        second.join();

        std::cout << user << "\n";
    }

    return 0;
}
