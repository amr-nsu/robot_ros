#pragma once

#include <eigen3/Eigen/Dense>

class Control
{
public:
    Control();
    Eigen::Vector2d getControl(const Eigen::Vector3d& x);
    void setReference(const Eigen::Vector3d& x_ref);

private:
    Eigen::Vector3d dot(const Eigen::Vector3d& x);
    Eigen::Vector3d x_ref_;
    Eigen::Vector2d theta_;
    const Eigen::Matrix3d a_;
    const Eigen::Matrix2d k_;
    const double dt = 0.01;
};

Eigen::Vector2d saturation(Eigen::Vector2d u);
