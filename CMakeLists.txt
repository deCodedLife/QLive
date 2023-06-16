cmake_minimum_required(VERSION 3.14)

project(QLive LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick HttpServer)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/network.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/network.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/timer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/timer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/tobject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/tobject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/httpserver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/httpserver.cpp
)

add_library(QLive STATIC
  ${SOURCES}
  ${CMAKE_CURRENT_SOURCE_DIR}/src/qlive.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/qlive.h
)

target_link_libraries(QLive PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::HttpServer
)

include_directories(QLive src/core)
target_compile_definitions(QLive PRIVATE QLIVE_LIBRARY)

#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
