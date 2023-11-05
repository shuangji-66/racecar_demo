cmake_minimum_required(VERSION 3.0.2)
project(floor_set)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  floor_msg
)


catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES floor_set
 CATKIN_DEPENDS roscpp rospy std_msgs   
#  DEPENDS system_lib
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
 catkin_install_python(PROGRAMS
   src/car_teleop.py
   ##src/car_vel_controler.py
   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
 )

