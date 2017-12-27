#pragma once

#include "Control.hpp"
#include <QMainWindow>
#include <QTimer>
#include "ros/ros.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/PoseStamped.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();

    void update();

private:
    void sendCommand(double linear, double angular);
    void poseCallback(const nav_msgs::Odometry::ConstPtr& msg);
    void targetCallback(const geometry_msgs::PoseStamped::ConstPtr& msg);


    Ui::MainWindow *ui;
    ros::NodeHandle node;
    ros::Publisher cmd_vel_pub;
    ros::Subscriber sub_pose;
    ros::Subscriber sub_target;
    QTimer timer;
    bool ap = false;
    Control control;
};
