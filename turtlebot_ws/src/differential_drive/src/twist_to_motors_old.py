#!/usr/bin/env python
"""
   twist_to_motors - converts a twist message to motor commands.  Needed for navigation stack
   !!!NOTE_EDIT: I'VE EDITED THIS TO REPORT ANGULAR VEL AND LINEAR VEL INSTEAD OF THE ORIGINAL LVEL AND RVEL!!!
   
    Copyright (C) 2012 Jon Stephan. 
     
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
"""

import rospy
import roslib
import math
from std_msgs.msg import Float32
from geometry_msgs.msg import Twist 

#############################################################
#############################################################
class TwistToMotors():
#############################################################
#############################################################

    #############################################################
    def __init__(self):
    ############################################################# this initializes the publisher and twist subscriber
        rospy.init_node("twist_to_motors")
        nodename = rospy.get_name()
        rospy.loginfo("%s started" % nodename)
    
        self.w = rospy.get_param("~base_width", 0.2)
    
#        self.pub_lmotor = rospy.Publisher('lwheel_vtarget', Float32, queue_size=10)
#        self.pub_rmotor = rospy.Publisher('rwheel_vtarget', Float32, queue_size=10)
        self.pub_Amotor = rospy.Publisher('Awheel_vtarget', Float32, queue_size=10)   #we want the pure angular velocity of the output to control the Angular motor heading
        self.pub_Dmotor = rospy.Publisher('Dwheel_vtarget', Float32, queue_size=10) #we want the pure linear velocity of the output to control the Distance motor position
        rospy.Subscriber('cmd_vel', Twist, self.twistCallback) #the twist subscriber has angular velocities xyz, and linear velocities xyz. we want angv.z and linv.xy
    
    
        self.rate = rospy.get_param("~rate", 50)
        self.timeout_ticks = rospy.get_param("~timeout_ticks", 2)
        self.left = 0 #ANGLE
        self.right = 0 #DISTANCE
        
    #############################################################
    def spin(self):
    #############################################################
    
        r = rospy.Rate(self.rate)
        idle = rospy.Rate(10)
        then = rospy.Time.now()
        self.ticks_since_target = self.timeout_ticks
    
        ###### main loop  ######
        while not rospy.is_shutdown():
        
            while not rospy.is_shutdown() and self.ticks_since_target < self.timeout_ticks:
                self.spinOnce()
                r.sleep()
            idle.sleep()
                
    #############################################################
    def spinOnce(self):
    #############################################################
    
        # dx = (l + r) / 2
        # dr = (r - l) / w
            
#        self.right = 1.0 * self.dx + self.dr * self.w / 2 
#        self.left = 1.0 * self.dx - self.dr * self.w / 2
        if self.dx<0:
          self.right = -math.sqrt(self.dx*self.dx + self.dy*self.dy) #DISTANCE (M/S) we take the hypotenuse of linvelocity.x and linvelocity.y, not linz as that points upward
        else:
          self.right = math.sqrt(self.dx*self.dx + self.dy*self.dy) #DISTANCE (M/S) we take the hypotenuse of linvelocity.x and linvelocity.y, not linz as that points upward
        
        self.left = self.dr #ANGLE (RAD/S) we take the angvelocity.z to get the angular direction the robot should move
        #rospy.loginfo("publishing: (%d, %d)", self.left, self.right) 
                
        self.pub_Amotor.publish(self.left) #ANGLE PUBLISH
        self.pub_Dmotor.publish(self.right) #DIST PUBLISH
            
        self.ticks_since_target += 1

    #############################################################
    def twistCallback(self,msg):
    ############################################################# this is called every time twist is updated. 
        # rospy.loginfo("-D- twistCallback: %s" % str(msg))
        self.ticks_since_target = 0
        self.dx = msg.linear.x  #M/S
        self.dr = msg.angular.z #RAD/S
        self.dy = msg.linear.y  #M/S
    
#############################################################
#############################################################
if __name__ == '__main__':
    """ main """
    try:
        twistToMotors = TwistToMotors()
        twistToMotors.spin()
    except rospy.ROSInterruptException:
        pass
