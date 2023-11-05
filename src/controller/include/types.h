#pragma once
#include<iostream>
#include<vector>
#include<cmath>
#include<string>
#include "floor_msg/Floor.h"
namespace ns_controller
{
struct VehicleState
{
    double x;
    double y;
    double yaw;
    double v;
    double r;
    double a;
    double vx;
    double vy;
    double ax;
    double ay;
    VehicleState()
    {
        


    }

};
struct TrajectoryPoint
{
    double x;
    double y;
    double yaw;
    double curvature;
    double velocity;
};
typedef std::vector<TrajectoryPoint>Trajectory;
}