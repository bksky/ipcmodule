cmake_minimum_required(VERSION 3.1)
project(ipcmodule)

include_directories(${ROOT_PATH}/../interface)

file(GLOB SRC_FILES ${ROOT_PATH}/*.cpp)
set(PUBLIC_HEADERS ${ROOT_PATH}/../interface/ipc_base.h)

add_definitions("-DAGORA_EXPORT")

add_library(${PROJECT_NAME} SHARED ${PUBLIC_HEADERS} ${SRC_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION "${SDK_VERSION}"
        SOVERSION "${SDK_VERSION}"
        PUBLIC_HEADER "${PUBLIC_HEADERS}"
        ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
        PDB_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
        )