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


#include "World.h"
#include "PhysicalManager.h"
#include "PhysicalSphere.h"
#include "PhysicalStaticPlane.h"

#include <btBulletDynamicsCommon.h>


World::World()
        : mPhysicalManager(PhysicalManager::Instance())
{

}

World::~World()
{
    // TODO: Auto cleanup
}

void World::OnLogic()
{
    mPhysicalManager.Step();
}

void World::createBullshit()
{
    // Ground
    new PhysicalStaticPlane(btVector3(0, 1, 0), 0, btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));

    // Wall
    new PhysicalStaticPlane(btVector3(1, 0, 0), 0, btTransform(btQuaternion(0, 0, 0, 1), btVector3(-10, 0, 0)));

    // 2 balls
    PhysicalSphere *s1 = new PhysicalSphere(1, 1, {0, 0, 0},
                                            btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 10, 0)));
    PhysicalSphere *s2 = new PhysicalSphere(2, 1.5, {0, 0, 0},
                                            btTransform(btQuaternion(0, 0, 0, 1), btVector3(5, 16, 3)));

    // Pavé
    PhysicalSphere *s3 = new PhysicalSphere(2, 1.5, {0, 0, 0},
                                            btTransform(btQuaternion(0, 0, 0, 1), btVector3(5, 16, 3)));

    // Constraints
    btPoint2PointConstraint *c = new btPoint2PointConstraint(*s1->mRigidBody,
                                                             *s2->mRigidBody,
                                                             btVector3(0, 0, 0),
                                                             btVector3(0, 5, 0));
    c->setDbgDrawSize(btScalar(5.f));
    mPhysicalManager.mDynamicsWorld->addConstraint(c, false);
}
