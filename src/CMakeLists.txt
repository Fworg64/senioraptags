add_executable(senioraptags main.cpp)

set(AT_LIBS apriltags ${OpenCV_LIBS})

find_package(Eigen3 REQUIRED)
include_directories(${Eigen3_INCLUDE_DIRS})

target_link_libraries(senioraptags ${Eigen3_LIBRARY} ${AT_LIBS})
