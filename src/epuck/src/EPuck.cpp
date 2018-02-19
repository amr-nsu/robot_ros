#include "EPuck.hpp"
#include <cmath>
#include <QDebug>

EPuck::EPuck()
/* epuck 3422 /dev/rfcomm0
 * epuck 3430 /dev/rfcomm1
 */
{
    serial.setPortName("/dev/rfcomm0");
    serial.setBaudRate(115200);
    if (!serial.open(QIODevice::ReadWrite)) {
        throw std::runtime_error("Device not opened");
    }
    serial.write("\n");
    command("D,0,0\n");
    sub_cmd_vel = node.subscribe("/robot/cmd_vel", 1, &EPuck::Cmd_VelCallback, this);
}

void EPuck::command(const QString& cmd)
{
    serial.write(cmd.toLatin1());
    serial.flush();
    qDebug() << cmd;
}

void EPuck::Cmd_VelCallback(const geometry_msgs::Twist::ConstPtr& msg)
/* D,d1,d2\n Advanced sercom protocol set motors speed command
 */
{
    constexpr auto wheel_radius = 0.02;
    constexpr auto wheel_base = 0.054;
    constexpr auto Kv = 500 / (M_PI * wheel_radius);
    constexpr auto Kw = 250 * wheel_base / (M_PI * wheel_radius);

    constexpr auto scale_factor = 10.0;  // gazebo pioneer2dx velocity to e-puck for demonstration

    auto v = msg->linear.x / scale_factor;
    auto w = msg->angular.z;

    auto d1 = static_cast<int>(Kv * v - Kw * w);
    auto d2 = static_cast<int>(Kv * v + Kw * w);

    QString cmd;
    QTextStream out(&cmd);
    out << "D," << d1 << "," << d2 << "\n";

    command(cmd);
}
