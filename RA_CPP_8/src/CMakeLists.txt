cmake_minimum_required(VERSION 3.12)

project(RA_CPP_8)

# NOTE: Добавляем опцию, позволяющую собрать проект без conan.
option(USE_CONAN "Use conan" ON)

# NOTE: Добавляем опцию, позволяющую собрать проект без примера с сопрограммами.
option(WITH_COROUTINES "Enable the coroutines example" ON)

find_package(Threads)

if (USE_CONAN)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else()
    find_package(Boost REQUIRED thread)
endif()

add_executable(Storage storage.cpp)
target_compile_features(Storage PRIVATE cxx_std_17)
target_link_libraries(Storage PRIVATE Threads::Threads)

add_executable(Futures futures.cpp)
target_compile_features(Futures PRIVATE cxx_std_17)

# NOTE: Включаем boost::future с методом then()
target_compile_definitions(Futures
    PRIVATE
        BOOST_THREAD_PROVIDES_FUTURE
        BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION
)

if (USE_CONAN)
    target_link_libraries(Futures
        PRIVATE
            Threads::Threads
            ${CONAN_LIBS}
    )
else()
    target_link_libraries(Futures
        PRIVATE
            Threads::Threads
            Boost::thread
    )
endif()

if (WITH_COROUTINES)
    add_executable(Coroutines
        coroutines.cpp
        Generator.h
        Task.h
    )

    target_compile_features(Coroutines PRIVATE cxx_std_20)

    # NOTE: Включаем концепты для Clang и MSVC.
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        target_compile_options(Coroutines PRIVATE -fcoroutines-ts -stdlib=libc++)
        target_link_libraries(Coroutines PRIVATE c++)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        target_compile_options(Coroutines PRIVATE /await)
    endif()
endif()
