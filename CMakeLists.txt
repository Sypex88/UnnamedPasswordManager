cmake_minimum_required(VERSION 4.0)
project(untitled2)

set(CMAKE_CXX_STANDARD 20)
include_directories(${CMAKE_SOURCE_DIR})
add_executable(untitled2 main.cpp
        VaultCrypto.cpp
        VaultCrypto.h
        VaultStorage.cpp)
find_package(unofficial-sodium CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE unofficial-sodium::sodium)

