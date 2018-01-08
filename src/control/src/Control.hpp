#pragma once

#include <eigen3/Eigen/Dense>

class Control
{
public:
    Control();
    Eigen::Vector2d getControl(Eigen::Vector3d x);
    void setReference(Eigen::Vector3d x_ref);

private:
    Eigen::Vector3d x_ref_;
    const Eigen::Matrix3d a_;
};
