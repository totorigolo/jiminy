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

#include <sstream>
#include <iostream>
#include <cmath>

#include <Box2D/Box2D.h>

#include "Brain.hpp"


Brain::Brain() :
        mIteration(0),
        mSimulated(false),
        mFailed(false)
{
}

Brain::~Brain()
{
}

void Brain::Think()
{
    mSimulated = true;
    std::cout << "\r#" << std::setw(7) << ++mIteration << ", ";

    onThink();

    std::cout << std::flush;
}

void Brain::Save() const
{
    this->onSave();
    std::cout << "Saved." << std::endl;
}

bool Brain::Failed() const
{
    return mFailed;
}

void Brain::Reseted()
{
    mFailed = false;
    mIteration = 0;
    this->onReseted();
}
