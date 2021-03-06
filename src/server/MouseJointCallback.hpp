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

#include <Box2D/Box2D.h>


class MouseJointCallback : public b2QueryCallback
{
public:
    MouseJointCallback(const b2Vec2 &point);

    bool ReportFixture(b2Fixture *fixture) override;

    const b2AABB &GetAABB() const;

    b2Fixture* GetFixture() const;

private:
    b2AABB mAABB;
    b2Vec2 mPoint;
    b2Fixture *mFixture;
};
