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
#include <cmath>

#include <Box2D/Box2D.h>

#include "Brain.hpp"


Brain::Brain() :
        mPIDkP{6},
        mPIDkI{1},
        mPIDkD{-6}
{}

void Brain::Think()
{
    float theta = mInfo["theta"]() * (180.f / b2_pi);// + 90.f;
    float thetaP = mInfo["theta."]();

    float currentError = -theta;

    mPIDintegral = (1 / 60.f) * (mPIDintegral + currentError);
    float derivative = (60.f) * (currentError - mPIDpreviousError);
    float force = mPIDkP * currentError + mPIDkI * mPIDintegral + mPIDkP * derivative;
    mPIDpreviousError = currentError;

//    float force = -theta * 3.f - thetaP * 20.f;
    std::cout << "theta=" << theta << ", "
              << "thetaP=" << thetaP << ", "
              << "force=" << force << std::endl;

    mActions["x."](force);
//    mActions["theta."](thetaP);
}
