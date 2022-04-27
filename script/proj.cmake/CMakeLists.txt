cmake_minimum_required(VERSION 3.1)
project(ipc)

get_filename_component(ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../src/ ABSOLUTE)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(common.cmake)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)
set(VERSION_TWEAK 0)
set(SDK_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${VERSION_TWEAK}")


set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/output")

add_subdirectory(ipcmodule)
add_subdirectory(ipcsender)
add_subdirectory(ipcreceiver)
add_subdirectory(ipcreceiverElectronVideo)

set(ALL_TARGETS ipcmodule ipcmodule ipcsender ipcreceiver ipcreceiverElectronVideo)

install(TARGETS ${ALL_TARGETS}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include)