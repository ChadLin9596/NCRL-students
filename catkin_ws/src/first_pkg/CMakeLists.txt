cmake_minimum_required(VERSION 2.8.3) 
project(first_pkg)
find_package(catkin REQUIRED COMPONENTS 
roscpp 
rospy 
std_msgs genmsg
)
catkin_package(
)
include_directories(include ${catkin_INCLUDE_DIRS})  
## build
add_executable(atest src/test.cpp) 
target_link_libraries(atest ${catkin_LIBRARIES})

add_executable(talker src/talker.cpp)
target_link_libraries(talker ${catkin_LIBRARIES})

add_executable(listener src/listener.cpp) 
target_link_libraries(listener ${catkin_LIBRARIES})