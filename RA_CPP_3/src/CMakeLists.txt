cmake_minimum_required(VERSION 3.12)

project(RA_CPP_3)

find_package(Threads)
find_package(Boost 1.67.0 REQUIRED system)

add_executable(Filesystem filesystem.cpp)
target_compile_features(Filesystem PRIVATE cxx_std_17)
target_link_libraries(Filesystem PRIVATE stdc++fs)

add_executable(Parser parser.cpp)
target_compile_features(Parser PRIVATE cxx_std_17)

add_executable(Strings strings.cpp)
target_compile_features(Strings PRIVATE cxx_std_17)

add_executable(Streams streams.cpp)
target_compile_features(Streams PRIVATE cxx_std_17)

target_link_libraries(Streams
    PRIVATE
        Boost::system
        Threads::Threads
)
