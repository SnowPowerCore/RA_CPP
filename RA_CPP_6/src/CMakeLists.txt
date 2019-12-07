cmake_minimum_required(VERSION 3.12)

project(RA_CPP_6)

# NOTE: Добавляем опцию, позволяющую собрать проект без примера с Qt.
option(WITH_QT_EXAMPLE "Enable the example of Qt application" ON)

if (WITH_QT_EXAMPLE)
    # NOTE: Включаем Qt-специфичную автогенерацию кода. Подробности в документации Qt.
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)

    find_package(Qt5 5.12 REQUIRED Widgets)
endif()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(Animals animals.cpp)
target_compile_features(Animals PRIVATE cxx_std_17)

add_executable(EchoServer echo_server.cpp)
target_compile_features(EchoServer PRIVATE cxx_std_17)
target_link_libraries(EchoServer PRIVATE ${CONAN_LIBS})

if (WITH_QT_EXAMPLE)
    add_executable(GuiApp gui_app.cpp MainWindow.cpp)
    target_compile_features(GuiApp PRIVATE cxx_std_17)
    target_link_libraries(GuiApp PRIVATE Qt5::Widgets)
endif()

add_executable(Optional optional.cpp)
target_compile_features(Optional PRIVATE cxx_std_17)

add_executable(Pointers pointers.cpp)
target_compile_features(Pointers PRIVATE cxx_std_17)

add_executable(Settings settings.cpp)
target_compile_features(Settings PRIVATE cxx_std_17)

add_executable(Storage storage.cpp)
target_compile_features(Storage PRIVATE cxx_std_17)
