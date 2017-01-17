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


Limb::Limb(std::weak_ptr<Entity> entity, b2BodyDef bodyDef, b2FixtureDef fixtureDef, b2World &b2World) :
        mEntity(entity), mb2Body{b2World.CreateBody(&bodyDef)}
{
    assert(fixtureDef.shape && "There must be a shape attached to the fixtureDef.");

    mb2Body->CreateFixture(&fixtureDef);
    mb2Body->SetUserData(this);
}

std::ostream &operator<<(std::ostream &os, const Limb &l)
{
    if (l.mb2Body)
    {
        os << "x = " << l.mb2Body->GetPosition().x << ", "
           << "y = " << l.mb2Body->GetPosition().y << ", "
           << "angle = " << l.mb2Body->GetAngle();
    }
    return os;
}
