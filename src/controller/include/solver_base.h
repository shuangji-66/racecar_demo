#pragma once
#include "floor_msg/Floor.h"
#include "types.h"


namespace ns_controller
{
    class Solver
    {
        public:
        void setTrajectory(const Trajectory &trajectory);
        void setState(const VehicleState& state);

        Trajectory predictive_path;
        std::vector<double>getresult();
        virtual void solve() = 0;
        protected:
        Trajectory trajectory_;
        VehicleState state_;
        floor_msg::Floor control_commend_;
    };
}