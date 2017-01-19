// This file is part of Jiminy.
// 
// Jiminy is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Jiminy is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Jiminy.  If not, see <http://www.gnu.org/licenses/>.


#pragma once

#include <memory>
#include <thread>
#include <atomic>

#include <Box2D/Box2D.h>


class Simulation
{
public:
    Simulation();
    Simulation(Simulation const &) = delete;
    Simulation &operator=(Simulation const &) = delete;

    virtual ~Simulation();

    int run();

    bool isSimulating() const;

    std::shared_ptr<b2World> GetB2World();

private:
    void simulate();
    void listenCommandLine();

private:
    std::shared_ptr<b2World> mB2World;
    std::atomic_bool mSimulating;

    std::unique_ptr<std::thread> mSimulationThread;
    std::unique_ptr<std::thread> mCommandLineThread;
};
