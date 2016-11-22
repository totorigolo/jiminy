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


#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletCollision/CollisionShapes/btStaticPlaneShape.h>
#include <LinearMath/btDefaultMotionState.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>

#include "PhysicalStaticPlane.h"
#include "DebugTools.h"


PhysicalStaticPlane::PhysicalStaticPlane(const btVector3 &planeNormal, const btScalar planeConstant, const btTransform &startTrans,
                         const btTransform &centerOfMassOffset)
        : mCollisionShape(new btStaticPlaneShape(planeNormal, planeConstant)),
          mMotionState(new btDefaultMotionState(startTrans, centerOfMassOffset)),
          mConstructionInfo(new btRigidBody::btRigidBodyConstructionInfo(0, mMotionState, mCollisionShape))
{
    REPORT();

    mConstructionInfo->m_restitution = 0.8f;

    mRigidBody = new btRigidBody(*mConstructionInfo);
    Register();
}

PhysicalStaticPlane::~PhysicalStaticPlane()
{
    REPORT();
}
