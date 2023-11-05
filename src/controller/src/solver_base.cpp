#pragma once
#include "solver_base.h"

namespace ns_controller
{
    void Solver::setTrajectory(const Trajectory &trajectory)
    {
        this->trajectory_=trajectory;
    }
    void Solver::setState(const VehicleState &state)
    {
        this->state_=state;
    }

    std::vector<double> Solver::getresult()
    {
        std::vector<double> output;
        output.emplace_back(control_commend_.angle);
        output.emplace_back(control_commend_.headlight);
        output.emplace_back(control_commend_.horn);
        output.emplace_back(control_commend_.speed);
        output.emplace_back(control_commend_.stop);
        return output;
    }

}