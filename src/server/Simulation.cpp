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


#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include <iostream>

#include "Simulation.h"
#include "World.h"
#include "PhysicalManager.h"
#include "DebugTools.h"

#include "PhysicalSphere.h"


using namespace std::chrono_literals;


Simulation::Simulation()
        : mSimulating(false),
          mSimulationThread(nullptr), mCommandLineThread(nullptr)
{
    REPORT();

    mWorld = new World;
}

Simulation::~Simulation()
{
    REPORT();

    mSimulating = false;

    if (mSimulationThread->joinable())
        mSimulationThread->join();
    if (mCommandLineThread->joinable())
        mCommandLineThread->join();

    delete mWorld;
}

int Simulation::run()
{
    REPORT();

//    PhysicalManager::Instance().EnableDebugDraw();

    mSimulating = true;

    mSimulationThread = new std::thread(&Simulation::simulate, this);
    mCommandLineThread = new std::thread(&Simulation::listenCommandLine, this);

    if (mSimulationThread->joinable())
        mSimulationThread->join();
    if (mCommandLineThread->joinable())
        mCommandLineThread->join();

    return 0;
}

bool Simulation::isSimulating() const
{
    return mSimulating;
}

void Simulation::simulate()
{
    REPORT();

    mWorld->createBullshit();

    while (isSimulating())
    {
        mWorld->OnLogic();

        PhysicalManager::Instance().DebugDraw();

        // TODO: Better framerate control
        std::this_thread::sleep_for(20ms);
    }
}

void Simulation::listenCommandLine()
{
    REPORT();

    std::string line, action;
    while (isSimulating())
    {
        std::getline(std::cin, line);
        std::stringstream ss;
        ss << line;
        ss >> action;

        if (action == "quit")
        {
            std::cout << "Bye !" << std::endl;
            mSimulating = false;
        }
        else if (action == "help")
        {
            std::cout << "Lol, find by yourself. But don't, DON'T, ask Thomas." << std::endl;
        }
        else if (action == "debug_draw" || action == "dd")
        {
            std::string param;
            ss >> param;
            if (param == "on" || param == "1")
            {
                PhysicalManager::Instance().EnableDebugDraw();
            }
            else if (param == "off" || param == "0")
            {
                PhysicalManager::Instance().DisableDebugDraw();
            }
            else
            {
                std::cout << "Parameter \"" << param << "\" unknown. Choose among: on, off or 0, 1." << std::endl;
            }
        }
        else if (action == "sphere" || action == "s")
        {
            btScalar r, x, y, z;
            ss >> r >> x >> y >> z;

            new PhysicalSphere(r, 1, {0, 0, 0}, btTransform(btQuaternion(0, 0, 0, 1), btVector3(x, y, z)));
        }
        else
        {
            std::cout << "Unknown action \"" << action << "\" (from \"" << line << "\")." << std::endl;
            std::cout << "Type \"help\" to know the available commands." << std::endl;
        }
    }
}
