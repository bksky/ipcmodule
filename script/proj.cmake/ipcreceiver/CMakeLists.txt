cmake_minimum_required(VERSION 3.1)
project(ipcreceiver)

include_directories(${ROOT_PATH}/../interface)

set(SRC_FILES ${ROOT_PATH}/../app/ipcreceiver/src/receiver.cpp)
set(DEPENDS ipcmodule)

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} ${DEPENDS})
