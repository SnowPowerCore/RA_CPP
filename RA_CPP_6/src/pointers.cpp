#include <iostream>
#include <memory>
#include <vector>

#define REQUIRES(...) typename = std::enable_if_t<__VA_ARGS__>

namespace
{
    template<typename Tree, typename Callable, REQUIRES(std::is_invocable_v<Callable, Tree>)>
    void traverse(Tree tree, Callable&& visit)
    {
        visit(tree);

        for (size_t i = 0, count = tree->childCount(); i < count; ++i) {
            traverse(tree->child(i), visit);
        }
    }
}

template<typename T>
class Tree : public std::enable_shared_from_this<Tree<T>>
{
public:
    static std::shared_ptr<Tree> create(T value)
    {
        return std::shared_ptr<Tree>(new Tree(std::move(value)));
    }

    ~Tree()
    {
        std::cout << "Deleted " << this << " " << value_ << "\n";
    }

    Tree(const Tree& other) = delete;
    Tree(Tree&& other) noexcept = default;

    void insert(std::shared_ptr<Tree> tree)
    {
        tree->parent_ = this->weak_from_this();
        children_.push_back(std::move(tree));
    }

    template<typename... Ts, REQUIRES((std::is_same_v<T, Ts> && ...))>
    Tree& insert(Ts... args)
    {
        (insert(Tree::create(std::move(args))), ...);
        return *children_.back();
    }

    std::shared_ptr<Tree> removeChild(size_t index)
    {
        std::shared_ptr<Tree> tree;

        if (index < children_.size()) {
            tree = std::move(children_[index]);
            tree->parent_.reset();

            children_.erase(std::next(children_.begin(), static_cast<int>(index)));
        }

        return tree;
    }

    T value() const
    {
        return value_;
    }

    std::shared_ptr<Tree> parent() const
    {
        return parent_.lock();
    }

    std::shared_ptr<Tree> child(size_t index) const
    {
        return children_[index];
    }

    size_t childCount() const
    {
        return children_.size();
    }

    size_t depth() const
    {
        size_t depth = 0;
        auto tree = this->shared_from_this();

        while ((tree = tree->parent())) {
            ++depth;
        }

        return depth;
    }

    void print()
    {
        traverse(this->shared_from_this(), [](std::shared_ptr<Tree> tree) {
            std::cout << std::string(tree->depth(), '-') << " " << tree->value() << "\n";
        });
    }

private:
    explicit Tree(T value)
        : value_(std::move(value))
    {}

private:
    T value_;
    std::weak_ptr<Tree> parent_;
    std::vector<std::shared_ptr<Tree>> children_;
};

int main()
{
    auto tree = Tree<int>::create(0);
    tree->insert(1);
    tree->insert(10).insert(11, 12, 19);
    tree->insert(20);
    tree->insert(30).insert(33, 31, 37).insert(35, 111).insert(-190);
    tree->insert(40).insert(42);

    tree->print();
    std::cout << "\n";

    {
        auto subtree = tree->child(3);
        subtree->print();
        std::cout << "\n";
    }

    tree->print();

    return 0;
}
