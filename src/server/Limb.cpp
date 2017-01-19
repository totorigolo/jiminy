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

#include <cassert>
#include <iostream>

#include "Limb.hpp"

#include "Entity.hpp"


Limb::Limb(b2BodyDef bodyDef, b2FixtureDef fixtureDef, std::shared_ptr<b2World> b2World) :
        mB2Body{b2World->CreateBody(&bodyDef)}, mB2World{b2World}
{
    assert(fixtureDef.shape && "There must be a shape attached to the fixtureDef.");
    assert(b2World && "The world can't be null.");

    mB2Body->CreateFixture(&fixtureDef);
    mB2Body->SetUserData(this);
}

Limb::~Limb()
{
    assert(!mB2World.expired() && mB2Body && "b2World and b2Body should not be null.");

    auto world(mB2World.lock());
    if (world)
    {
        world->DestroyBody(mB2Body);
    }
}

std::ostream &operator<<(std::ostream &os, const Limb &l)
{
    if (l.mB2Body)
    {
        os << "x = " << l.mB2Body->GetPosition().x << ", "
           << "y = " << l.mB2Body->GetPosition().y << ", "
           << "angle = " << l.mB2Body->GetAngle();
    }
    return os;
}
