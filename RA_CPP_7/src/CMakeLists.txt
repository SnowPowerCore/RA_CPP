cmake_minimum_required(VERSION 3.12)

project(RA_CPP_7)

find_package(OpenMP REQUIRED)
find_package(Threads REQUIRED)

option(USE_CONAN "Use conan" ON)

if (USE_CONAN)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else()
    find_package(OpenCV REQUIRED)
endif()

add_executable(Deadlock deadlock.cpp)
target_compile_features(Deadlock PRIVATE cxx_std_17)
target_link_libraries(Deadlock PRIVATE Threads::Threads)

add_executable(MapReduce map_reduce.cpp)
target_compile_features(MapReduce PRIVATE cxx_std_17)

if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    target_link_libraries(MapReduce PRIVATE tbb)
endif()

add_executable(Matching matching.cpp)
target_compile_features(Matching PRIVATE cxx_std_17)

if (USE_CONAN)
    target_link_libraries(Matching
        PRIVATE
            OpenMP::OpenMP_CXX
            ${CONAN_LIBS}
    )
else()
    target_link_libraries(Matching
        PRIVATE
            OpenMP::OpenMP_CXX
            ${OpenCV_LIBS}
    )
endif()

add_executable(ParallelAlgorithm parallel_algorithm.cpp)
target_compile_features(ParallelAlgorithm PRIVATE cxx_std_17)

if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    target_link_libraries(ParallelAlgorithm PRIVATE tbb)
endif()

add_executable(Philosofers philosofers.cpp)
target_compile_features(Philosofers PRIVATE cxx_std_17)
target_link_libraries(Philosofers PRIVATE Threads::Threads)
