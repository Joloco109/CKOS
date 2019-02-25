include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(CKOSBase STATIC KMath.cpp Logger.cpp MissionInfo.cpp StreamLogger.cpp)
target_link_libraries(CKOSBase PUBLIC ${kprc})
