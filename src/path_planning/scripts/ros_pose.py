#!/usr/bin/env python

import rospy

from std_msgs.msg import Header
from geometry_msgs.msg import PoseStamped


class Ros_Pose:

    def __init__(self):
        self.local_position_pose_pub = rospy.Publisher('/target/pose',
                                                       PoseStamped, queue_size=1)

    def publish(self, x, y):
        pos = PoseStamped()
        pos.header = Header()
        pos.header.frame_id = 'target_pose'
        pos.pose.position.x = x
        pos.pose.position.y = y
        self.local_position_pose_pub.publish(pos)
