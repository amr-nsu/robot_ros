#!/usr/bin/env python

import rospy

from std_msgs.msg import Header
from nav_msgs.msg import Odometry


class Ros_Pose:

    def __init__(self):
        self.target_pose_pub = rospy.Publisher('/target/pose',
                                               Odometry, queue_size=1)

    def publish(self, x, y):
        msg = Odometry()
        msg.header = Header()
        msg.header.frame_id = 'target_pose'
        msg.pose.pose.position.x = x
        msg.pose.pose.position.y = y
        self.target_pose_pub.publish(msg)
