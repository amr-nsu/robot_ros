#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <QTextStream>
#include "geometry_msgs/Twist.h"
#include "tf/transform_datatypes.h"
#include <angles/angles.h>

double getYaw(const nav_msgs::Odometry::ConstPtr &msg)
{
    tf::Quaternion q;
    tf::quaternionMsgToTF(msg->pose.pose.orientation, q);
    double roll, pitch, yaw;
    tf::Matrix3x3(q).getRPY(roll, pitch, yaw);
    return angles::normalize_angle(yaw);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow{parent},
    ui{new Ui::MainWindow}
{
    ui->setupUi(this);
    cmd_vel_pub = node.advertise<geometry_msgs::Twist>("/pioneer2dx/cmd_vel", 1);
    sub_pose = node.subscribe("/pioneer2dx/odom", 1, &MainWindow::poseCallback, this);
    sub_target = node.subscribe("target/pose", 1, &MainWindow::targetCallback, this);
    timer.setInterval(10);  // ms
    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.start();
}

void MainWindow::targetCallback(const geometry_msgs::PoseStamped::ConstPtr& msg)
{
    auto x = msg->pose.position.x;
    auto y = msg->pose.position.y;
    control.setReference(Eigen::Vector3d{x, y, 0});
}

void MainWindow::poseCallback(const nav_msgs::Odometry::ConstPtr& msg)
{
    QString pose;
    QTextStream out(&pose);
    out.setRealNumberPrecision(3);
    auto x = msg->pose.pose.position.x;
    auto y = msg->pose.pose.position.y;
    out << "x :" << x << " y: " << y;
    double psi = getYaw(msg);
    out << " psi :" << psi  << " AP: "  << ap;
    ui->label->setText(pose);
    if (ap) {
        auto u = control.getControl(Eigen::Vector3d{x, y, psi});
        sendCommand(u(0), u(1));
    }
}

void MainWindow::update()
{
    ros::spinOnce();
}

void MainWindow::sendCommand(double linear, double angular)
{
    geometry_msgs::Twist msg;
    msg.linear.x = linear;
    msg.angular.z = angular;
    cmd_vel_pub.publish(msg);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    sendCommand(0.5, 0);
}

void MainWindow::on_pushButton_2_clicked()
{
    sendCommand(0.5, 0.5);
}

void MainWindow::on_pushButton_3_clicked()
{
    sendCommand(0.5, -0.5);
}

void MainWindow::on_pushButton_4_clicked()
{
    sendCommand(-0.5, 0);
}

void MainWindow::on_pushButton_5_clicked()
{
    sendCommand(0, 0);
}

void MainWindow::on_pushButton_6_clicked()
{
    ap = !ap;
}
