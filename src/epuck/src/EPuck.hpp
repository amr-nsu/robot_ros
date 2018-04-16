#pragma once
#include <QSerialPort>
#include "geometry_msgs/Twist.h"
#include "ros/ros.h"

class EPuck
{
public:
    EPuck(const QString& device);
private:
    ros::Subscriber sub_cmd_vel;
    ros::NodeHandle node;
    QSerialPort serial;
    void sendCommand(const QString& cmd);
    void cmdVelCallback(const geometry_msgs::Twist::ConstPtr& msg);
};
