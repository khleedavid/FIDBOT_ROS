execute_process(COMMAND "/home/jerom/turtlebot_ws/build/differential_drive/catkin_generated/python_distutils_install.sh" RESULT_VARIABLE res)

if(NOT res EQUAL 0)
  message(FATAL_ERROR "execute_process(/home/jerom/turtlebot_ws/build/differential_drive/catkin_generated/python_distutils_install.sh) returned error code ")
endif()
