#include <iostream>
#include <thread>
#include <chrono>

#include "rclcpp/rclcpp.hpp"
#include "unitree_api/msg/request.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "common/ros2_sport_client.h"
#include "std_msgs/msg/bool.hpp"

using namespace std::placeholders;

class CmdVelToSportRequest : public rclcpp::Node
{
public:
    CmdVelToSportRequest() : Node("cmd_vel_to_sport_request")
    {
        cmd_vel_subscriber_ = this->create_subscription<geometry_msgs::msg::Twist>(
            "/cmd_vel", 10, std::bind(&CmdVelToSportRequest::cmdVelCallback, this, _1));

        emergency_subscriber_ = this->create_subscription<std_msgs::msg::Bool>(
            "/emergency_switch", 10, std::bind(&CmdVelToSportRequest::emergencyCallback, this, _1));

        request_publisher_ = this->create_publisher<unitree_api::msg::Request>("/api/sport/request", 10);
    }

private:
    bool emergency_flag = false;

    void cmdVelCallback(const geometry_msgs::msg::Twist::SharedPtr cmd_vel_msg)
    {
        unitree_api::msg::Request req;

        if (!emergency_flag) {
            // zで立ち上がり・しゃがみ
            if (std::abs(cmd_vel_msg->linear.z) >= 0.1) {

                if (cmd_vel_msg->linear.z > 0.0) {
                    sport_client_.StandUp(req);
                    request_publisher_->publish(req);

                    std::this_thread::sleep_for(std::chrono::seconds(3));

                    sport_client_.BalanceStand(req);
                    request_publisher_->publish(req);

                } else {
                    sport_client_.StandDown(req);
                    request_publisher_->publish(req);
                }

            } else {
                sport_client_.Move(req,
                    cmd_vel_msg->linear.x,
                    cmd_vel_msg->linear.y,
                    cmd_vel_msg->angular.z);

                request_publisher_->publish(req);
            }

        } else {
            sport_client_.BalanceStand(req);
            request_publisher_->publish(req);
        }
    }

    void emergencyCallback(const std_msgs::msg::Bool::SharedPtr emergency_msg)
    {
        emergency_flag = emergency_msg->data;
    }

    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_subscriber_;
    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr emergency_subscriber_;
    rclcpp::Publisher<unitree_api::msg::Request>::SharedPtr request_publisher_;

    SportClient sport_client_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<CmdVelToSportRequest>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}