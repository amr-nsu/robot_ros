#pragma once
#include <QSerialPort>
#include "geometry_msgs/Twist.h"
#include "ros/ros.h"

class EPuck
{
public:
    EPuck();
private:
    ros::Subscriber sub_cmd_vel;
    ros::NodeHandle node;
    QSerialPort serial;
    void command(const QString& cmd);
    void Cmd_VelCallback(const geometry_msgs::Twist::ConstPtr& msg);
};
