cmake_minimum_required(VERSION 3.12)

project(RA_CPP_3)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# NOTE: Поиск системной библиотеки поддержки потоков.
find_package(Threads)

add_executable(Filesystem filesystem.cpp)
target_compile_features(Filesystem PRIVATE cxx_std_17)

add_executable(Parser parser.cpp)
target_compile_features(Parser PRIVATE cxx_std_17)

target_link_libraries(Parser
    PRIVATE
        Threads::Threads # NOTE: Компонуем цель с системной библиотекой потоков.
        ${CONAN_LIBS}
)

add_executable(Strings strings.cpp)
target_compile_features(Strings PRIVATE cxx_std_17)

add_executable(Streams streams.cpp)
target_compile_features(Streams PRIVATE cxx_std_17)

target_link_libraries(Streams
    PRIVATE
        Threads::Threads
        ${CONAN_LIBS}
)
