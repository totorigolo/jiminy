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

#include <ostream>
#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

class Entity;
class Limb
{
public:
    Limb(b2BodyDef bodyDef, b2FixtureDef fixtureDef, std::shared_ptr<b2World> b2World);

    friend class Entity; // TODO: Remove

public:
    Limb(Limb const &) = delete;

    Limb &operator=(Limb const &) = delete;

    virtual ~Limb();

public:
    b2Body *mB2Body; // TODO: Make mB2Body private
private:
    std::weak_ptr<b2World> mB2World;

    std::vector<Limb> mArticulations; // TODO: Traduction ?

public:
    friend std::ostream &operator<<(std::ostream &os, const Limb &l);
};
