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

#include <BulletCollision/btBulletCollisionCommon.h>
#include <LinearMath/btIDebugDraw.h>
#include <mutex>
#include <SFML/Window.hpp>


class DebugDrawer : public btIDebugDraw
{
public:

    DebugDrawer();

    virtual ~DebugDrawer();

    virtual void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) override;

    virtual void
    drawPlane(const btVector3 &planeNormal, btScalar planeConst, const btTransform &transform, const btVector3 &color) override;

    virtual void
    drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime,
                     const btVector3 &color) override;

    virtual void reportErrorWarning(const char *warningString) override;

    virtual void draw3dText(const btVector3 &location, const char *textString) override;

    virtual void setDebugMode(int debugMode) override;

    virtual int getDebugMode() const override;

    void render(btCollisionWorld *bulletWorld);

private:
    bool onPreRender();
    void onPostRender();

    void createWindow();
    void destroyWindow();

private:
    int mDebugMode;
    std::mutex mRenderingMutex;
    sf::Window *mWindow;
    bool mWindowHasFocus;
    sf::Vector3f mEyePos;
    sf::Vector3f mTargetPos;
};
