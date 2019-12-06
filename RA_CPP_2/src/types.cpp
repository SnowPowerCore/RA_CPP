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

    int b = a; // копирование
    b = -1;
    assert(a == 42 && b == -1);

    int& lvref = a; // ссылка на переменную (lvalue reference)
    lvref = -1;
    assert(a == -1 && lvref == -1);

    int&& rvref = getValue(); // ссылка на временный объект (rvalue reference)
    int&& rvref_ = 42; // или на литерал

    int&& rvref__ = std::move(a); // или присвоение другой ссылки
    rvref__ = -1;
    assert(a == -1 && rvref__ == -1);

    a = 42;
    int* p = &a; // взятие адреса переменной в памаяти (формирование указателя)
    std::cout << p << " " << *p << "\n";

    *p = -1; // разыменовывание указателя
    assert(a == -1 && *p == -1);

    p += 10;
    std::cout << p << " " << *p << "\n";

    p = nullptr; // указатель в никуда
    std::cout << p << "\n";

    // провека типов (вывод информации о типе переменной в stdout)
    std::cout << typeid(rvref_).name() << "\n";
    std::cout << boost::typeindex::type_id<decltype(rvref__)>().pretty_name() << "\n";
    std::cout << boost::typeindex::type_id_with_cvr<decltype(rvref__)>().pretty_name() << "\n";

    return 0;
}
