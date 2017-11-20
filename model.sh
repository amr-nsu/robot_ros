#!/bin/bash

cd ${0%/*}

rosrun gazebo_ros gazebo src/model/robot.world
killall -9 gzserver
