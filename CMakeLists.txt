cmake_minimum_required(VERSION 3.10)
project(CelenganKu)

set(CMAKE_CXX_STANDARD 14) 

include(FetchContent)


set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/arthursonzogni/ftxui.git
  GIT_TAG        v5.0.0
  GIT_PROGRESS   TRUE
  GIT_SHALLOW    TRUE
  EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(ftxui)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)


# find_package(ftxui CONFIG REQUIRED)

set(SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/src")
set(INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include")

# set client variables
# set(SRC_CLIENT "${SRC_DIR}/client/*.cpp")
file(GLOB SRC_CLIENT "${SRC_DIR}/client/*.cpp")
file(GLOB UI "${SRC_DIR}/client/ui/*.cpp")
set(CLIENT "${PROJECT_NAME}_client")

file(GLOB SRC_SHARED "${SRC_DIR}/shared/*.cpp")

# set server variables
file(GLOB SRC_SERVER 
    "${SRC_DIR}/server/*.cpp"
)
set(SERVER "${PROJECT_NAME}_server")

# create client executable
add_executable(${CLIENT} ${SRC_CLIENT} ${SRC_SHARED} ${UI})
target_include_directories(${CLIENT} PRIVATE "${INCLUDE_DIR}/shared")
target_include_directories(${CLIENT} PRIVATE "${INCLUDE_DIR}/client")

add_executable(${SERVER} ${SRC_SERVER} ${SRC_SHARED})
target_include_directories(${SERVER} PRIVATE "${INCLUDE_DIR}/shared")
target_include_directories(${SERVER} PRIVATE "${INCLUDE_DIR}/server")


if(WIN32)
    target_link_libraries(${CLIENT} PRIVATE ws2_32)
endif()

target_link_libraries(${CLIENT} PRIVATE ftxui::dom ftxui::screen ftxui::component nlohmann_json::nlohmann_json)
set_target_properties(${CLIENT} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

if(WIN32)
    target_link_libraries(${SERVER} PRIVATE ws2_32)
endif()

set_target_properties(${SERVER} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)