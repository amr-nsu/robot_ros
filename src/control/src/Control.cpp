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

    auto saturation = [](Eigen::Vector2d u) {
        if (u(0) > 1.0) u(0) = 1.0;
        else if (u(0) < -1.0) u(0) = - 1.0;
        return u;
    };

    return saturation(f.transpose() * F);
}

void Control::setReference(const Eigen::Vector3d& x_ref)
{
    x_ref_ = x_ref;
}
