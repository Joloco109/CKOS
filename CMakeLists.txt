cmake_minimum_required (VERSION 3.1)

project (CKOS)

set( CMAKE_EXPORT_COMPILE_COMMANDS ON)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++11 -lkrpc -lprotobuf -pthread")
set (source_dir "${PROJECT_SOURCE_DIR}/src/")


file (GLOB source_files "${source_dir}/*.cpp")

add_executable (${PROJECT_NAME} ${source_files} )
#add_executable (${PROJECT_NAME} src/main.cpp src/Logger.cpp )

target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/include/krpc")
target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/include/spdlog")
#link_directories("${PROJECT_SOURCE_DIR}/lib/krpc")

target_link_libraries(${PROJECT_NAME} krpc)
