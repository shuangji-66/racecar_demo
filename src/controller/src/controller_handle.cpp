/*
    Formula Student Driverless Project (FSD-Project).
    Copyright (c) 2020:
     - chentairan <tairanchen@bitfsd.cn>

    FSD-Project is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    FSD-Project is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FSD-Project.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <ros/ros.h>
#include "controller_handle.hpp"

namespace ns_controller
{

  // Constructor
  controlHandle::controlHandle(ros::NodeHandle &nodeHandle) : nodeHandle_(nodeHandle),
                                                              controller_(nodeHandle)
  {
    ROS_INFO("Constructing Handle");
    loadParameters();
    subscribeToTopics();
    publishToTopics();
  }

  // Getters
  int controlHandle::getNodeRate() const { return node_rate_; }

  // Methods
  void controlHandle::loadParameters()
  {
    ROS_INFO("loading handle parameters");
    if (!nodeHandle_.param<std::string>("input_imu_topic_name",
                                        input_imu_topic_name_,
                                        "/wit/imu"))
    {
      ROS_WARN_STREAM("Did not load input_imu_topic_name. Standard value is: " << input_imu_topic_name_);
    }
    if (!nodeHandle_.param<std::string>("input_location_topic_name",
                                        input_location_topic_name_,
                                        "/wit/location"))
    {
      ROS_WARN_STREAM("Did not load input_location_topic_name. Standard value is: " << input_location_topic_name_);
    }

    if (!nodeHandle_.param<std::string>("output_pose_topic_name",
                                        output_pose_topic_name_,
                                        "/cmd"))
    {
      ROS_WARN_STREAM("Did not load output_pose_topic_name. Standard value is: " << output_pose_topic_name_);
    }
    if (!nodeHandle_.param("node_rate", node_rate_, 1))
    {
      ROS_WARN_STREAM("Did not load node_rate. Standard value is: " << node_rate_);
    }
  }
  // ros::Subscriber inputimuSubscriber_;
  // ros::Subscriber inputlocationSubscriber_;
  // ros::Subscriber inputmagSubscriber_;
  // ros::Publisher ouputPosePublisher_;

  // void GetImuMessagecallback(const sensor_msgs::Imu::ConstPtr& msgs);
  // void GetLocationMessagecallback(const sensor_msgs::NavSatFix::ConstPtr& msgs);
  // void GetMagMessagecallback(const sensor_msgs::MagneticField::ConstPtr& msgs);

  void controlHandle::subscribeToTopics()
  {
    ROS_INFO("subscribe to topics");
    inputimuSubscriber_ = nodeHandle_.subscribe(input_imu_topic_name_, 10, &controlHandle::GetImuMessagecallback, this);
    inputlocationSubscriber_ = nodeHandle_.subscribe(input_location_topic_name_, 10, &controlHandle::GetLocationMessagecallback, this);
 
  }

  void controlHandle::publishToTopics()
  {
    ROS_INFO("publish to topics");
    ouputPosePublisher_ = nodeHandle_.advertise<floor_msg::Floor>(output_pose_topic_name_, 1);
  }

  void controlHandle::run()
  {
    controller_.runAlgorithm();
    sendMsg();
  }

  void controlHandle::sendMsg()
  {
    ouputPosePublisher_.publish(controller_.sendcmd());
  }


     void controlHandle::GetImuMessagecallback(const sensor_msgs::Imu::ConstPtr& msgs)
     {
      controller_.setimu(msgs);
     }
   void controlHandle::GetLocationMessagecallback(const sensor_msgs::NavSatFix::ConstPtr& msgs)
   {
    controller_.setlocation(msgs);
   }
 
}