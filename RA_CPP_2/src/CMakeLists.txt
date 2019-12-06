cmake_minimum_required(VERSION 3.12)

project(RA_CPP_2)

# NOTE: Подключаем conan к проекту.
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(array array.cpp ByteArray.cpp)

# NOTE: Включаем поддержку C++17 для цели.
target_compile_features(array PRIVATE cxx_std_17)

add_executable(inheritance inheritance.cpp)
target_compile_features(inheritance PRIVATE cxx_std_17)

add_executable(polymorphism polymorphism.cpp)
target_compile_features(polymorphism PRIVATE cxx_std_17)

add_executable(types types.cpp)
target_compile_features(types PRIVATE cxx_std_17)

# NOTE: Компонуем цель с зависимостями из conan.
target_link_libraries(types PRIVATE ${CONAN_LIBS})
