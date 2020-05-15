# Install script for directory: /home/jerom/turtlebot_ws/src/move_base_flex/mbf_msgs

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/jerom/turtlebot_ws/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/mbf_msgs/srv" TYPE FILE FILES
    "/home/jerom/turtlebot_ws/src/move_base_flex/mbf_msgs/srv/CheckPoint.srv"
    "/home/jerom/turtlebot_ws/src/move_base_flex/mbf_msgs/srv/CheckPose.srv"
    "/home/jerom/turtlebot_ws/src/move_base_flex/mbf_msgs/srv/CheckPath.srv"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/mbf_msgs/action" TYPE FILE FILES
    "/home/jerom/turtlebot_ws/src/move_base_flex/mbf_msgs/action/GetPath.action"
    "/home/jerom/turtlebot_ws/src/move_base_flex/mbf_msgs/action/ExePath.action"
    "/home/jerom/turtlebot_ws/src/move_base_flex/mbf_msgs/action/Recovery.action"
    "/home/jerom/turtlebot_ws/src/move_base_flex/mbf_msgs/action/MoveBase.action"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/mbf_msgs/msg" TYPE FILE FILES
    "/home/jerom/turtlebot_ws/devel/share/mbf_msgs/msg/GetPathAction.msg"
    "/home/jerom/turtlebot_ws/devel/share/mbf_msgs/msg/GetPathActionGoal.msg"
    "/home/jerom/turtlebot_ws/devel/share/mbf_msgs/msg/GetPathActionResult.msg"
    "/home/jerom/turtlebot_ws/devel/share/mbf_msgs/msg/GetPathActionFeedback.msg"
    "/home/jerom/turtlebot_ws/devel/share/mbf_msgs/msg/GetPathGoal.msg"
    "/home/jerom/turtlebot_ws/devel/share/mbf_msgs/msg/GetPathResult.msg"
    "/home/jerom/turtlebot_ws/devel/share/mbf_msgs/msg/GetPathFeedback.msg"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/mbf_msgs/msg" TYPE FILE FILES
    "/home/jerom/turtlebot_ws/devel/share/mbf_msgs/msg/ExePathAction.msg"
    "/home/jerom/turtlebot_ws/devel/share/mbf_msgs/msg/ExePathActionGoal.msg"
    "/home/jerom/turtlebot_ws/devel/share/mbf_msgs/msg/ExePathActionResult.msg"
    "/home/jerom/turtlebot_ws/devel/share/mbf_msgs/msg/ExePathActionFeedback.msg"
    "/home/jerom/turtlebot_ws/devel/share/mbf_msgs/msg/ExePathGoal.msg"
    "/home/jerom/turtlebot_ws/devel/share/mbf_msgs/msg/ExePathResult.msg"
    "/home/jerom/turtlebot_ws/devel/share/mbf_msgs/msg/ExePathFeedback.msg"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/mbf_msgs/msg" TYPE FILE FILES
    "/home/jerom/turtlebot_ws/devel/share/mbf_msgs/msg/RecoveryAction.msg"
    "/home/jerom/turtlebot_ws/devel/share/mbf_msgs/msg/RecoveryActionGoal.msg"
    "/home/jerom/turtlebot_ws/devel/share/mbf_msgs/msg/RecoveryActionResult.msg"
    "/home/jerom/turtlebot_ws/devel/share/mbf_msgs/msg/RecoveryActionFeedback.msg"
    "/home/jerom/turtlebot_ws/devel/share/mbf_msgs/msg/RecoveryGoal.msg"
    "/home/jerom/turtlebot_ws/devel/share/mbf_msgs/msg/RecoveryResult.msg"
    "/home/jerom/turtlebot_ws/devel/share/mbf_msgs/msg/RecoveryFeedback.msg"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/mbf_msgs/msg" TYPE FILE FILES
    "/home/jerom/turtlebot_ws/devel/share/mbf_msgs/msg/MoveBaseAction.msg"
    "/home/jerom/turtlebot_ws/devel/share/mbf_msgs/msg/MoveBaseActionGoal.msg"
    "/home/jerom/turtlebot_ws/devel/share/mbf_msgs/msg/MoveBaseActionResult.msg"
    "/home/jerom/turtlebot_ws/devel/share/mbf_msgs/msg/MoveBaseActionFeedback.msg"
    "/home/jerom/turtlebot_ws/devel/share/mbf_msgs/msg/MoveBaseGoal.msg"
    "/home/jerom/turtlebot_ws/devel/share/mbf_msgs/msg/MoveBaseResult.msg"
    "/home/jerom/turtlebot_ws/devel/share/mbf_msgs/msg/MoveBaseFeedback.msg"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/mbf_msgs/cmake" TYPE FILE FILES "/home/jerom/turtlebot_ws/build/move_base_flex/mbf_msgs/catkin_generated/installspace/mbf_msgs-msg-paths.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/home/jerom/turtlebot_ws/devel/include/mbf_msgs")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/roseus/ros" TYPE DIRECTORY FILES "/home/jerom/turtlebot_ws/devel/share/roseus/ros/mbf_msgs")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/common-lisp/ros" TYPE DIRECTORY FILES "/home/jerom/turtlebot_ws/devel/share/common-lisp/ros/mbf_msgs")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gennodejs/ros" TYPE DIRECTORY FILES "/home/jerom/turtlebot_ws/devel/share/gennodejs/ros/mbf_msgs")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  execute_process(COMMAND "/usr/bin/python2" -m compileall "/home/jerom/turtlebot_ws/devel/lib/python2.7/dist-packages/mbf_msgs")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python2.7/dist-packages" TYPE DIRECTORY FILES "/home/jerom/turtlebot_ws/devel/lib/python2.7/dist-packages/mbf_msgs")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/jerom/turtlebot_ws/build/move_base_flex/mbf_msgs/catkin_generated/installspace/mbf_msgs.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/mbf_msgs/cmake" TYPE FILE FILES "/home/jerom/turtlebot_ws/build/move_base_flex/mbf_msgs/catkin_generated/installspace/mbf_msgs-msg-extras.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/mbf_msgs/cmake" TYPE FILE FILES
    "/home/jerom/turtlebot_ws/build/move_base_flex/mbf_msgs/catkin_generated/installspace/mbf_msgsConfig.cmake"
    "/home/jerom/turtlebot_ws/build/move_base_flex/mbf_msgs/catkin_generated/installspace/mbf_msgsConfig-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/mbf_msgs" TYPE FILE FILES "/home/jerom/turtlebot_ws/src/move_base_flex/mbf_msgs/package.xml")
endif()

