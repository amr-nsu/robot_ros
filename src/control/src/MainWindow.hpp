#pragma once

#include "Control.hpp"
#include <QMainWindow>
#include <QTimer>
#include "ros/ros.h"
#include "nav_msgs/Odometry.h"

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
    void on_forwardButton_clicked();
    void on_backwardButton_clicked();
    void on_leftButton_clicked();
    void on_rightButton_clicked();
    void on_stopButton_clicked();
    void on_apButton_clicked();
    void update();

private:
    void sendCommand(double linear, double angular);
    void poseCallback(const nav_msgs::Odometry::ConstPtr& msg);
    void targetCallback(const nav_msgs::Odometry::ConstPtr& msg);

    Ui::MainWindow *ui;
    ros::NodeHandle node;
    ros::Publisher cmd_vel_pub;
    ros::Subscriber sub_pose;
    ros::Subscriber sub_target;
    QTimer timer;
    bool ap = false;
    Control control;
};
