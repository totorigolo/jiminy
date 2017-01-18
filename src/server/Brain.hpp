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

#include <map>
#include <string>
#include <functional>


class Brain
{
public:
    Brain();

    void Think();

    // TODO: Make mActions private
    std::map<std::string, std::function<float(void)>> mInfo;
    std::map<std::string, std::function<void(float)>> mActions;
private:
    float mPIDkP;
    float mPIDkI;
    float mPIDkD;
    float mPIDpreviousError;
    float mPIDintegral;
};

