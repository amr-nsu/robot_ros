#include <QApplication>
#include "EPuck.hpp"
#include "ros/ros.h"

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "control");
    QApplication a(argc, argv);
    EPuck epuck;
    return a.exec();
}
