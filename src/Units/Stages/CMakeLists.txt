set (SOURCE ${SOURCE} 
	${CMAKE_CURRENT_SOURCE_DIR}/MissionStage.cpp 
) 

set (HEADER ${HEADER} 
	${CMAKE_CURRENT_SOURCE_DIR}/MissionStage.h 
) 


add_executable (${PROJECT_NAME}_AscentToOrbit ${SOURCE} 
	${CMAKE_CURRENT_SOURCE_DIR}/Ascent.cpp  
	${CMAKE_CURRENT_SOURCE_DIR}/Node.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OrbitalInsertion.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/AscentToOrbitTest.cpp
	)
add_executable (${PROJECT_NAME}_Lander ${SOURCE} 
	${CMAKE_CURRENT_SOURCE_DIR}/SuicideBurn.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/LanderTest.cpp)

#message("Using kRPC at location: ${krpc}")

find_library(krpc krpc)
#target_link_libraries(${PROJECT_NAME}_AscentToOrbit PUBLIC CKOSBase "${kprc}")
target_link_libraries(${PROJECT_NAME}_AscentToOrbit PUBLIC CKOSBase /usr/local/lib/libkrpc.so)
#target_link_libraries(${PROJECT_NAME}_Lander PUBLIC CKOSBase "${kprc}")
target_link_libraries(${PROJECT_NAME}_Lander PUBLIC CKOSBase /usr/local/lib/libkrpc.so)
