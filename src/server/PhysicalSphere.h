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

#include "Physical3DPrimitive.h"


class PhysicalSphere : public Physical3DPrimitive
{
public:
    PhysicalSphere(btScalar radius, btScalar mass, btVector3 fallInertia = btVector3(0, 0, 0),
                   const btTransform &startTrans = btTransform::getIdentity(),
                   const btTransform &centerOfMassOffset = btTransform::getIdentity());

    virtual ~PhysicalSphere();

private:
    btCollisionShape *mCollisionShape;
    btDefaultMotionState *mMotionState;
    btRigidBody::btRigidBodyConstructionInfo *mConstructionInfo;
};