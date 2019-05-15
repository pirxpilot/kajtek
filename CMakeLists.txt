cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Name of the project (will be the name of the plugin)
project(kajtek)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(${PROJECT_NAME} SHARED)

target_sources(${PROJECT_NAME}
  PRIVATE
    "src/kajtek.cc"
    "src/kajtek.h"
    "src/ref.h"
    "src/workers.cc"
    "src/workers.h"
    "src/tinycdb.cc"
    "src/tinycdb.h"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  # require modern C++
  CXX_STANDARD 17
  # Gives our library file a .node extension without any "lib" prefix
  PREFIX ""
  SUFFIX ".node"
)

# Essential include files to build a node addon,
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC})

# Essential library files to link to a node addon
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})

# tinycdb

set(TINYCDB_BUILD_CLI OFF)

add_subdirectory(deps/tinycdb)

# include_directories(SYSTEM "${PROJECT_SOURCE_DIR}/deps/tinycdb/include")
target_link_libraries(${PROJECT_NAME} tinycdb)
