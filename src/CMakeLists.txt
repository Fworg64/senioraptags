add_executable(senioraptags main.cpp)

add_executable(testjobber hello.c)

include_directories(../include) #I dont think this is the best way to do this

set(AT_LIBS apriltags ${OPENCV_LDFLAGS})

add_library(can_sensors can_sensors.cpp)

add_library(position_can_sensor position_can_sensor.cpp)

add_library(dynamixel_control dynamixel_control.c)

target_link_libraries(position_can_sensor can_sensors)

find_package(Eigen3 REQUIRED)
include_directories(${Eigen3_INCLUDE_DIRS})

target_link_libraries(testjobber dynamixel_control)

target_link_libraries(senioraptags dynamixel_control position_can_sensor ${Eigen3_LIBRARY} ${AT_LIBS})
