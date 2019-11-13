cmake_minimum_required(VERSION 3.12)

project(RA_CPP_2)

find_package(Boost 1.67.0 REQUIRED system)

add_executable(array array.cpp ByteArray.cpp)
target_compile_features(array PRIVATE cxx_std_17)

add_executable(inheritance inheritance.cpp)
target_compile_features(inheritance PRIVATE cxx_std_17)

add_executable(polymorphism polymorphism.cpp)
target_compile_features(polymorphism PRIVATE cxx_std_17)

add_executable(types types.cpp)
target_compile_features(types PRIVATE cxx_std_17)
target_link_libraries(types PRIVATE Boost::system)
