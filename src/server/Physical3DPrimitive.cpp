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


#include "Physical3DPrimitive.h"
#include "PhysicalManager.h"
#include "DebugTools.h"


Physical3DPrimitive::Physical3DPrimitive()
        : mRegistered(false), mRigidBody(nullptr)
{
    REPORT();
}

Physical3DPrimitive::~Physical3DPrimitive()
{
    REPORT();

    PhysicalManager::Instance().RemoveObject(this);
    mRegistered = false;
}

void Physical3DPrimitive::Register()
{
    REPORT();

    PhysicalManager::Instance().RegisterObject(this);
    mRegistered = true;
}
