cmake_minimum_required(VERSION 3.19)
project(CelenganKu)

set(CMAKE_CXX_STANDARD 14) 

set(SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/src")
set(INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include")

# set client variables
set(SRC_CLIENT "${SRC_DIR}/client/main.cpp")
set(CLIENT "${PROJECT_NAME}_client")

# set server variables
set(SRC_SERVER "${SRC_DIR}/server/main.cpp")
set(SERVER "${PROJECT_NAME}_server")

# create client executable
add_executable(${CLIENT} ${SRC_CLIENT})
target_include_directories(${CLIENT} PRIVATE "${INCLUDE_DIR}/shared")
set_target_properties(${CLIENT} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)