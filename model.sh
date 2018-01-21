#!/bin/bash

cd ${0%/*}

source devel/setup.bash || exit 1

rosrun gazebo_ros gazebo src/model/robot.world
killall -9 gzserver
