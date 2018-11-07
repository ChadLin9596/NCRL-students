# %Tag(FULLTEXT)%
cmake_minimum_required(VERSION 2.8.3)
 add_compile_options(-std=c++11)
project(second_pkg)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs genmsg)


## Generate added messages and services
generate_messages(DEPENDENCIES std_msgs)

## Declare a catkin package
catkin_package()

## Build talker and listener
include_directories(include ${catkin_INCLUDE_DIRS})

## add_executable([your node name] src/[your file name].cpp)
## target_link_libraries(listener ${catkin_LIBRARIES})
## add_dependencies(listener beginner_tutorials_generate_messages_cpp)
# %EndTag(FULLTEXT)%

add_executable(tutorial_1 src/tutorial_1.cpp)
target_link_libraries(tutorial_1 ${catkin_LIBRARIES})
add_dependencies(tutorial_1 beginner_tutorials_generate_messages_cpp)

add_executable(tutorial_2 src/tutorial_2.cpp)
target_link_libraries(tutorial_2 ${catkin_LIBRARIES})
add_dependencies(tutorial_2 beginner_tutorials_generate_messages_cpp)

add_executable(tutorial_3 src/tutorial_3.cpp)
target_link_libraries(tutorial_3 ${catkin_LIBRARIES})
add_dependencies(tutorial_3 beginner_tutorials_generate_messages_cpp)