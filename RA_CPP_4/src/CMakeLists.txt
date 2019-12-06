cmake_minimum_required(VERSION 3.12)

project(RA_CPP_4)

add_executable(Templates templates.cpp)
target_compile_features(Templates PRIVATE cxx_std_17)

add_executable(Storage storage.cpp)
target_compile_features(Storage PRIVATE cxx_std_17)

add_executable(Sfinae sfinae.cpp)
target_compile_features(Sfinae PRIVATE cxx_std_17)

add_executable(Polymorphism polymorphism.cpp)
target_compile_features(Polymorphism PRIVATE cxx_std_17)

# NOTE: Включаем концепты для GCC.
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_executable(Concepts concepts.cpp)
    target_compile_features(Concepts PRIVATE cxx_std_20)
    target_compile_options(Concepts PRIVATE "-fconcepts")
endif()
