cmake_minimum_required(VERSION 3.12)

project(RA_CPP_6)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 5.12 REQUIRED Widgets)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(Animals animals.cpp)
target_compile_features(Animals PRIVATE cxx_std_17)

add_executable(EchoServer echo_server.cpp)
target_compile_features(EchoServer PRIVATE cxx_std_17)
target_link_libraries(EchoServer PRIVATE ${CONAN_LIBS})

add_executable(GuiApp gui_app.cpp MainWindow.cpp)
target_compile_features(GuiApp PRIVATE cxx_std_17)
target_link_libraries(GuiApp PRIVATE Qt5::Widgets)

add_executable(Optional optional.cpp)
target_compile_features(Optional PRIVATE cxx_std_17)

add_executable(Pointers pointers.cpp)
target_compile_features(Pointers PRIVATE cxx_std_17)

add_executable(Settings settings.cpp)
target_compile_features(Settings PRIVATE cxx_std_17)

add_executable(Storage storage.cpp)
target_compile_features(Storage PRIVATE cxx_std_17)
