#include "EPuck.hpp"
#include <QDebug>

EPuck::EPuck()
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
{
    constexpr auto scale_factor = 10.0;

    auto v = msg->linear.x / scale_factor;
    auto w = msg->angular.z;

    constexpr auto Kv = 500 / (3.14 * 0.02);
    constexpr auto Kw = 250 * 0.054 / (3.14 * 0.02);

    auto d1 = static_cast<int>(Kv * v - Kw * w);
    auto d2 = static_cast<int>(Kv * v + Kw * w);

    QString cmd;
    QTextStream out(&cmd);
    out << "D," << d1 << "," << d2 << "\n";

    command(cmd);
}
