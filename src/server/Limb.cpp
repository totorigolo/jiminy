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

#include "Limb.h"
#include "sf2b2.hpp"


Limb::Limb(b2BodyDef bodyDef, b2FixtureDef fixtureDef, b2World &b2World) :
        mb2Body{b2World.CreateBody(&bodyDef)}
{
    assert(fixtureDef.shape && "There must be a shape attached to the fixtureDef.");

    mb2Body->CreateFixture(&fixtureDef);
    mb2Body->SetUserData(this);

    switch (fixtureDef.shape->m_type)
    {
        case b2Shape::e_circle:
            mShape = std::make_unique<sf::CircleShape>(fixtureDef.shape->m_radius * PPM);
            break;
        case b2Shape::e_polygon:
        {
            mShape = std::make_unique<sf::ConvexShape>();
            b2PolygonShape *polyShape{(b2PolygonShape *) fixtureDef.shape};
            sf::ConvexShape *convex{(sf::ConvexShape *) mShape.get()};
            convex->setPointCount((size_t) polyShape->m_count);
            for (size_t i = 0; i < convex->getPointCount(); ++i)
            {
                convex->setPoint(i, convert(polyShape->GetVertex((int32) i)));
            }
        }
            break;
        default:
            throw std::runtime_error("Unhandled shape");
    }

    switch (bodyDef.type)
    {
        case b2_dynamicBody:
            mShape->setFillColor(sf::Color::Green);
            break;
        case b2_staticBody:
            mShape->setFillColor(sf::Color::Red);
            break;
        case b2_kinematicBody:
            mShape->setFillColor(sf::Color::Magenta);
            break;
    }
}

void Limb::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (mShape && mb2Body)
    {
        if (mb2Body->GetFixtureList()->GetShape()->m_type == b2Shape::e_circle)
            mShape->setPosition(convert(mb2Body->GetPosition())
                                + sf::Vector2f(mShape->getLocalBounds().width, mShape->getLocalBounds().height) / 2.f);
        else
            mShape->setPosition(convert(mb2Body->GetPosition()));

        target.draw(*mShape, states);
    }
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
