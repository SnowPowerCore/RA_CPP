#include <deque>
#include <iostream>
#include <vector>

namespace
{
    template<typename T, bool IsLarge>
    struct Node;

    template<typename T>
    struct Node<T, true>
    {
        using value_type = T*;

        T* value;

        Node()
            : value(nullptr)
        {}

        Node(T value)
            : value(new T(std::move(value)))
        {}

        Node(const Node& other)
            : value(new T)
        {
           *value = *other.value;
        }

        Node(Node&& other) noexcept
            : value(nullptr)
        {
            std::swap(value, other.value);
        }

        ~Node()
        {
            delete value;
        }

        Node& operator=(const Node& other)
        {
            return *this = Node(other);
        }

        Node& operator=(Node&& other) noexcept = default;
    };

    template<typename T>
    struct Node<T, false>
    {
        using value_type = T;

        T value;

        Node(T value = T())
            : value(std::move(value))
        {}
    };
}

template<typename T, template<typename, typename> typename Container = std::vector>
class List
{
    using node_type = Node<T, (sizeof(T) > sizeof(T*))>;
    using container_type = Container<node_type, std::allocator<node_type>>;

public:
    using value_type = T;
    using size_type = typename container_type::size_type;
    using reference = T&;
    using const_reference = const T&;

public:
    List() = default;

    List(std::initializer_list<T> ilist)
    {
        container_.resize(ilist.size());
        std::move(ilist.begin(), ilist.end(), container_.begin());
    }

    reference operator[](size_type pos)
    {
        if constexpr (std::is_same_v<value_type, typename node_type::value_type>) {
            return container_[pos].value;
        } else {
            return *(container_[pos].value);
        }
    }

    const_reference operator[](size_type pos) const
    {
        if constexpr (std::is_same_v<value_type, typename node_type::value_type>) {
            return container_[pos].value;
        } else {
            return *(container_[pos].value);
        }
    }

    bool empty() const { return container_.empty(); }
    size_type size() const { return container_.size(); }

    void clear() { container_.clear(); }
    void push_back(value_type&& value) { container_.push_back(std::forward<value_type>(value)); }

private:
    container_type container_;
};

int main()
{
    List<int> list = { 1, 2, 3, 4, 5};
    list[3] = 456;
    list.push_back(34);
    list.push_back(30);

    std::cout << std::boolalpha << list.empty() << " " << list.size() << " " << list[0] << list[5] << "\n";

    List<std::string, std::deque> strings = {"One", "Two"};
    strings.push_back("Three");
    strings.push_back("Four");
    strings[0] = "New text";

    std::cout << std::boolalpha << strings.empty() << " " << strings.size() << " " << strings[0] << strings[3] << "\n";

    return 0;
}
