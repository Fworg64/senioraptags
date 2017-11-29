add_executable(senioraptags main.cpp)

include_directories(../include) #I dont think this is the best way to do this

set(AT_LIBS apriltags ${OPENCV_LDFLAGS})

add_library(can_sensors can_sensors.cpp)

add_library(position_can_sensor position_can_sensor.cpp)

target_link_libraries(position_can_sensor can_sensors)

find_package(Eigen3 REQUIRED)
include_directories(${Eigen3_INCLUDE_DIRS})

target_link_libraries(senioraptags position_can_sensor ${Eigen3_LIBRARY} ${AT_LIBS})