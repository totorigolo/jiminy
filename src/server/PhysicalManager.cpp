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


#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>

#include "PhysicalManager.h"
#include "Physical3DPrimitive.h"
#include "DebugTools.h"
#include "DebugDrawer.h"


const float PhysicalManager::GRAVITY = -9.81f;

PhysicalManager::PhysicalManager()
{
    REPORT();

    // Build the broadphase
    mBroadphase = new btDbvtBroadphase();

    // Set up the collision configuration and dispatcher
    mCollisionConfiguration = new btDefaultCollisionConfiguration();
    mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);

    // The actual physics solver
    mSolver = new btSequentialImpulseConstraintSolver;

    // The world
    mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mBroadphase, mSolver, mCollisionConfiguration);
    mDynamicsWorld->setGravity(btVector3(0, GRAVITY, 0));

    // Debug draw
    mDebugDrawer = new DebugDrawer();
    mDynamicsWorld->setDebugDrawer(mDebugDrawer);
//    mDebugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
}

PhysicalManager::~PhysicalManager()
{
    REPORT();

    for (auto &&primitive : m3DPrimitives)
    {
        mDynamicsWorld->removeRigidBody(primitive->mRigidBody);
    }
    m3DPrimitives.clear();

    delete mDynamicsWorld;
    delete mSolver;
    delete mDispatcher;
    delete mCollisionConfiguration;
    delete mBroadphase;
    delete mDebugDrawer;
}

void PhysicalManager::RegisterObject(Physical3DPrimitive *primitive3D)
{
    REPORT();

    m3DPrimitives.push_back(primitive3D);
    mDynamicsWorld->addRigidBody(primitive3D->mRigidBody);
}

bool PhysicalManager::RemoveObject(Physical3DPrimitive *primitive3D)
{
    REPORT();

    for (auto it = m3DPrimitives.begin(); it < m3DPrimitives.end(); ++it)
    {
        if (*it == primitive3D)
        {
            m3DPrimitives.erase(it);
            mDynamicsWorld->removeRigidBody(primitive3D->mRigidBody);
            return true;
        }
    }
    return false;
}

void PhysicalManager::Step()
{
//    REPORT();

    // TODO: Physics framerate
    mDynamicsWorld->stepSimulation(1 / 60.f, 10);
}

void PhysicalManager::EnableDebugDraw(int debugMode)
{
    mDebugDrawer->setDebugMode(debugMode);
}

void PhysicalManager::DisableDebugDraw()
{
    mDebugDrawer->setDebugMode(0);
}

void PhysicalManager::DebugDraw()
{
//    REPORT();

    mDebugDrawer->render(mDynamicsWorld);
}
