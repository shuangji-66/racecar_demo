#include "stanley_solver.h"
#include <cmath>
#include <vector>

namespace ns_controller
{
    double Stanley::PointDistanceSquare(const double &x, const double &y, const VehicleState &state)
    {
        double dx = x - state.x;
        double dy = y - state.y;
        return dx * dx + dy * dy;
    }
    int Stanley::findnearestindex(const Trajectory &traj, const VehicleState &state)
    {
        int index = 0;
        double d_min = PointDistanceSquare(traj.front().x, traj.front().y, state);
        for (int i = 1; i < traj.size(); ++i)
        {
            double d_temp = PointDistanceSquare(traj[i].x, traj[i].y, state);
            if (d_temp < d_min)
            {
                d_min = d_temp;
                index = i;
            }
        }
        return index;
    }
    void Stanley::solve()
    {
        //速度控制
        double desire_v=10;
        const double vel=state_.v;
        PIDController speedpid(0.2,0.2,0.1);
        control_commend_.speed=speedpid.Control(desire_v-vel,0.1);
        //横向控制
        int index=findnearestindex(trajectory_,state_);

    }

    void Stanley::ComputeControlCmd(const Trajectory &traj, const VehicleState &state)
    {
        double e_y_;
        double e_theta_;
        ComputeLateralErrors(traj,state.x,state.y,state.yaw,e_y_,e_theta_);
        double k_y_=0.8;
        double tra_steer=e_theta_+atan2((k_y_*e_y_),state.y);
            if (tra_steer > 1)
      tra_steer = 1;
    else if (tra_steer < -1)
      tra_steer = -1;
    else
    {
    }
    control_commend_.angle=tra_steer;

    }
    void Stanley::ComputeLateralErrors(const Trajectory &traj, const double x, const double y, const double theta,
                                       double &e_y, double &e_theta)
    {
            auto trajectory_points = traj[3];

    double dx = (trajectory_points.x - x);
    double dy = (trajectory_points.y - y);

    e_y = dy * cos(trajectory_points.yaw) - dx * sin(trajectory_points.yaw);

    e_theta = trajectory_points.yaw - theta;

    if (e_theta > M_PI)
    {
      e_theta = e_theta - M_PI * 2;
    }
    if (e_theta < -M_PI)
    {
      e_theta = e_theta + M_PI * 2;
    }
    }

}