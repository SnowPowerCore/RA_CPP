cmake_minimum_required(VERSION 3.12)

project(RA_CPP_7)

find_package(Threads)

add_executable(Deadlock deadlock.cpp)
target_compile_features(Deadlock PRIVATE cxx_std_17)
target_link_libraries(Deadlock PRIVATE Threads::Threads)

add_executable(MapReduce map_reduce.cpp)
target_compile_features(MapReduce PRIVATE cxx_std_17)
target_link_libraries(MapReduce PRIVATE tbb)

add_executable(ParallelAlgorithm parallel_algorithm.cpp)
target_compile_features(ParallelAlgorithm PRIVATE cxx_std_17)
target_link_libraries(ParallelAlgorithm PRIVATE tbb)

add_executable(Philosofers philosofers.cpp)
target_compile_features(Philosofers PRIVATE cxx_std_17)
target_link_libraries(Philosofers PRIVATE Threads::Threads)
