#include <QApplication>
#include "EPuck.hpp"
#include "ros/ros.h"

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "epuck");
    QApplication a(argc, argv);
    EPuck epuck;
    ros::spin();
}
