#include <ros/ros.h>

#include "ld06.hpp"

using namespace std;
using namespace boost;

int main(int argc, char **argv)
{
    ros::init(argc, argv, "ldlidar_nos");
    ros::NodeHandle nh;
    ros::NodeHandle priv_nh("~");

    std::string port;
    priv_nh.param("port", port, std::string("/dev/ttyUSB0"));
    ROS_INFO("port: %s", port.c_str());

    std::string frame_id;
    priv_nh.param("frame_id", frame_id, std::string("base_link"));
    ROS_INFO("frame_id: %s", frame_id.c_str());

    auto pub = nh.advertise<sensor_msgs::LaserScan>("scan", 1);
    while (nh.ok())
    {
        try
        {
            Serial serial(port, 230400);
            LD06 ld(serial, frame_id);
            while (nh.ok())
            {
                ld.process([&](const sensor_msgs::LaserScan &ls)
                           {
                               if (pub.getNumSubscribers() > 0)
                                   pub.publish(ls);
                           });
            }
        }
        catch (system::system_error &e)
        {
            //check if interrupt
            if (e.code().value() == 4)
            {
                //shut down
                return 0;
            }
            ROS_ERROR("%s", e.what());
            ROS_INFO("retry in 5sec");
            ros::Duration(5.0).sleep();
        }
    }
    return 0;
}
