#include <utility>

#include "ByteArray.h"

namespace
{
    ByteArray readFile()
    {
        return ByteArray(1024, static_cast<std::byte>('a'));
    }
}

int main()
{
    // creation
    ByteArray byteArray(1024);

    // copying
    ByteArray copied(byteArray);

    // copy elision
    ByteArray captured(readFile());

    // moving
    ByteArray moved(std::move(byteArray));

    return 0;
}
