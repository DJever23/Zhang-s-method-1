PROJECT(Zhang-s-method-1)
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
if(COMMAND camke_policy)
	camke_policy(SET CMP0003 NEW)
endif(COMMAND camke_policy)

FIND_PACKAGE( OpenCV REQUIRED)

#Declare teh target (an executable)
ADD_EXECUTABLE(cal1 CameraCalibration.cpp)
TARGET_LINK_LIBRARIES(cal1 ${OpenCV_LIBS})
#MESSAGE(STATUS "OpenCV_LIBS: ${OpenCV_LIBS}")
