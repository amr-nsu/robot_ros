#include "Control.hpp"
#include <cmath>
#include <angles/angles.h>

Control::Control() :
    a_{Eigen::DiagonalMatrix<double, 3>(1.0, 1.0, 2.0)}
{
}

Eigen::Vector2d Control::getControl(const Eigen::Vector3d& x)
{
    auto distance = (x_ref_ - x).head(2).norm();
    if (distance < 0.2) return Eigen::Vector2d{0, 0};

    Eigen::MatrixXd f(3, 2);
    f << cos(x(2)), 0,
         sin(x(2)), 0,
                 0, 1;

    Eigen::Vector3d F  = a_ * (x_ref_ - x);
    F(2) = angles::shortest_angular_distance(x(2), atan2(F(1), F(0)));

    theta_ += k_ * f.transpose() * (F - dot(x)) * dt;

    Eigen::Vector2d u = f.transpose() * F + theta_;

    return saturation(u);
}

void Control::setReference(const Eigen::Vector3d& x_ref)
{
    x_ref_ = x_ref;
}

Eigen::Vector3d Control::dot(const Eigen::Vector3d& x)
{
    static Eigen::Vector3d x_prev = x;
    Eigen::Vector3d dot_x = (x - x_prev) / dt;
    x_prev = x;
    return dot_x;
}

Eigen::Vector2d saturation(Eigen::Vector2d u)
{
    if (u(0) > 1.0) u(0) = 1.0;
    else if (u(0) < -1.0) u(0) = - 1.0;
    return u;
}
