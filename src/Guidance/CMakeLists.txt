include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(CKOSGuidance STATIC BrakeCalculator.cpp Interface.cpp)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Simulator)
