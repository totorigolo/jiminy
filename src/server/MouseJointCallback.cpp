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

#include "MouseJointCallback.hpp"

MouseJointCallback::MouseJointCallback(const b2Vec2& point) :
        mPoint{point}, mFixture{}
{
    mFixture = nullptr;

    // Create a tiny AABB
    b2Vec2 d(0.001f, 0.001f);
    mAABB.lowerBound = mPoint - d;
    mAABB.upperBound = mPoint + d;
}

bool MouseJointCallback::ReportFixture(b2Fixture* fixture)
{
    b2Body* body = fixture->GetBody();
    if (body->GetType() == b2_dynamicBody)
    {
        if (fixture->TestPoint(mPoint))
        {
            mFixture = fixture;
            return false;
        }
    }

    // Continue the search
    return true;
}

const b2AABB &MouseJointCallback::GetAABB() const
{
    return mAABB;
}

b2Fixture* MouseJointCallback::GetFixture() const
{
    return mFixture;
}
