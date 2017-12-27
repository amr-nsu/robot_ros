#include "Control.hpp"
#include <cmath>
#include <angles/angles.h>

Control::Control() :
    a_{Eigen::DiagonalMatrix<double, 3>(0.3, 0.3, 1.0)}
{
}

Eigen::Vector2d Control::getControl(Eigen::Vector3d x)
{
    Eigen::MatrixXd f(3, 2);
    f << cos(x(2)), 0,
         sin(x(2)), 0,
                 0, 1;

    Eigen::Vector3d F  = a_ * (x_ref_ - x);
    F(2) = angles::shortest_angular_distance(x(2), atan2(F(1), F(0)));

    return f.transpose() * F;
}

void Control::setReference(Eigen::Vector3d x_ref)
{
    x_ref_ = x_ref;
}
