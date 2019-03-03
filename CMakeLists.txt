cmake_minimum_required (VERSION 3.1)

project (CKOS)

set( CMAKE_EXPORT_COMPILE_COMMANDS ON)

#set(Boost_USE_STATIC_LIBS OFF) 
#set(Boost_USE_MULTITHREADED ON)  
#set(Boost_USE_STATIC_RUNTIME OFF)
#find_package(Boost 1.68 COMPONENTS uBLAS)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wpedantic -Wno-misleading-indentation -std=c++11 -lprotobuf -pthread")

link_directories("${PROJECT_SOURCE_DIR}/lib/krpc")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/include/krpc")
include_directories("${PROJECT_SOURCE_DIR}/include/spdlog")
include_directories("${PROJECT_SOURCE_DIR}/include/boost")

add_subdirectory(${PROJECT_SOURCE_DIR}/src)




#add_executable (${PROJECT_NAME}_AscentToOrbit ${SOURCE} ${PROJECT_SOURCE_DIR}/src/AscentToOrbitTest.cpp)
#add_executable (${PROJECT_NAME}_Lander ${SOURCE} ${PROJECT_SOURCE_DIR}/src/LanderTest.cpp)

#target_include_directories(${PROJECT_NAME}_AscentToOrbit PRIVATE "${PROJECT_SOURCE_DIR}/include")
#target_include_directories(${PROJECT_NAME}_AscentToOrbit PRIVATE "${PROJECT_SOURCE_DIR}/include/krpc")
#target_include_directories(${PROJECT_NAME}_AscentToOrbit PRIVATE "${PROJECT_SOURCE_DIR}/include/spdlog")
#target_include_directories(${PROJECT_NAME}_AscentToOrbit PRIVATE "${PROJECT_SOURCE_DIR}/include/boost")

#target_include_directories(${PROJECT_NAME}_Lander PRIVATE "${PROJECT_SOURCE_DIR}/include")
#target_include_directories(${PROJECT_NAME}_Lander PRIVATE "${PROJECT_SOURCE_DIR}/include/krpc")
#target_include_directories(${PROJECT_NAME}_Lander PRIVATE "${PROJECT_SOURCE_DIR}/include/spdlog")
#target_include_directories(${PROJECT_NAME}_Lander PRIVATE "${PROJECT_SOURCE_DIR}/include/boost")

#link_directories("${PROJECT_SOURCE_DIR}/lib/krpc")

#target_link_libraries(${PROJECT_NAME}_AscentToOrbit krpc)
#target_link_libraries(${PROJECT_NAME}_AscentToOrbit CKOSBase)
#target_link_libraries(${PROJECT_NAME}_Lander krpc)
#target_link_libraries(${PROJECT_NAME}_Lander CKOSBase)
