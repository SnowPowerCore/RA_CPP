cmake_minimum_required(VERSION 3.12)

project(RA_CPP_5)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(Functional functional.cpp)
target_compile_features(Functional PRIVATE cxx_std_17)

add_executable(Algorithm algorithm.cpp)
target_compile_features(Algorithm PRIVATE cxx_std_17)

add_executable(Iterator iterator.cpp)
target_compile_features(Iterator PRIVATE cxx_std_17)

add_executable(Lambdas lambdas.cpp)
target_compile_features(Lambdas PRIVATE cxx_std_17)

add_executable(List list.cpp)
target_compile_features(List PRIVATE cxx_std_17)

add_executable(Ranges ranges.cpp)
target_compile_features(Ranges PRIVATE cxx_std_17)
target_link_libraries(Ranges PRIVATE ${CONAN_LIBS})

add_executable(StlUsage stl_usage.cpp)
target_compile_features(StlUsage PRIVATE cxx_std_17)
