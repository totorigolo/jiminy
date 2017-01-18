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


#include <iostream>

#include <Box2D/Box2D.h>

#include "Simulation.hpp"


int main()
{
    srand((unsigned int) (time(NULL)));

    try
    {
        std::cout << "Box2D " << b2_version.major << "." << b2_version.minor << "." << b2_version.revision << std::endl;

        Simulation simulation;
        return simulation.run();
    }
    catch (std::exception e)
    {
        std::cerr << "An unhandled fatal exception just happened:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
