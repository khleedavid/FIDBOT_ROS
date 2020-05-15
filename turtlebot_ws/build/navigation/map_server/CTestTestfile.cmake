# CMake generated Testfile for 
# Source directory: /home/jerom/turtlebot_ws/src/navigation/map_server
# Build directory: /home/jerom/turtlebot_ws/build/navigation/map_server
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(_ctest_map_server_gtest_map_server_utest "/home/jerom/turtlebot_ws/build/catkin_generated/env_cached.sh" "/usr/bin/python2" "/opt/ros/melodic/share/catkin/cmake/test/run_tests.py" "/home/jerom/turtlebot_ws/build/test_results/map_server/gtest-map_server_utest.xml" "--return-code" "/home/jerom/turtlebot_ws/devel/lib/map_server/map_server_utest --gtest_output=xml:/home/jerom/turtlebot_ws/build/test_results/map_server/gtest-map_server_utest.xml")
add_test(_ctest_map_server_rostest_test_rtest.xml "/home/jerom/turtlebot_ws/build/catkin_generated/env_cached.sh" "/usr/bin/python2" "/opt/ros/melodic/share/catkin/cmake/test/run_tests.py" "/home/jerom/turtlebot_ws/build/test_results/map_server/rostest-test_rtest.xml" "--return-code" "/usr/bin/python2 /opt/ros/melodic/share/rostest/cmake/../../../bin/rostest --pkgdir=/home/jerom/turtlebot_ws/src/navigation/map_server --package=map_server --results-filename test_rtest.xml --results-base-dir \"/home/jerom/turtlebot_ws/build/test_results\" /home/jerom/turtlebot_ws/src/navigation/map_server/test/rtest.xml ")
