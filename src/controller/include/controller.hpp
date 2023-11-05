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

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "geometry_msgs/Pose2D.h"
#include "std_msgs/String.h"
#include"sensor_msgs/Imu.h"
#include"sensor_msgs/NavSatFix.h"
#include"sensor_msgs/MagneticField.h"
#include <ros/ros.h>
#include "floor_msg/Floor.h"
#include "types.h"
#include "solver_base.h"
#include "tf/transform_listener.h"
#include "tf/transform_datatypes.h"

namespace ns_controller {

class control {

 public:
  // Constructor
  control(ros::NodeHandle& nh);

	// Getters
floor_msg::Floor sendcmd();

	// Setters
  void setimu(const sensor_msgs::Imu::ConstPtr& msg);
  void setlocation(const sensor_msgs::NavSatFix::ConstPtr& msg);




  void runAlgorithm();

private:

	ros::NodeHandle& nh_;
	
	VehicleState carstate;
  std::vector<double>result;
    Solver* solver_;
    Trajectory refine_;
};
}

#endif //CONTROLLER_HPP
