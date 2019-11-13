#include <cassert>
#include <iostream>

#include <boost/type_index.hpp>

int getValue()
{
    return 42;
}

int main()
{
    int a = 42;

    int b = a; // copying
    b = -1;
    assert(a == 42 && b == -1);

    int& lvref = a; // referencing to a variable (lvalue reference)
    lvref = -1;
    assert(a == -1 && lvref == -1);

    int&& rvref = getValue(); // referencing to a temporary object (rvalue reference)
    int&& rvref_ = 42; // or a literal;

    int&& rvref__ = std::move(a); // or assign by other reference;
    rvref__ = -1;
    assert(a == -1 && rvref__ == -1);

    a = 42;
    int* p = &a; // taking the address of a variable (making the pointer)
    std::cout << p << " " << *p << "\n";

    *p = -1; // pointer dereference
    assert(a == -1 && *p == -1);

    p += 10;
    std::cout << p << " " << *p << "\n";

    p = nullptr; // pointing to nowhere
    std::cout << p << "\n";

    // type checking
    std::cout << typeid(rvref_).name() << "\n";
    std::cout << boost::typeindex::type_id<decltype(rvref__)>().pretty_name() << "\n";
    std::cout << boost::typeindex::type_id_with_cvr<decltype(rvref__)>().pretty_name() << "\n";

    return 0;
}
