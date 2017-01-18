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

#include "Entity.hpp"


Entity::Entity() :
    mColor{sf::Uint8(rand() % 256), sf::Uint8(rand() % 256), sf::Uint8(rand() % 256)}
{

}

void Entity::Think()
{
    mBrain.Think();

//    for (auto &limb: mLimbs)
//    {
//        limb->mB2Body->ApplyTorque(0.1f, true);
//    }
}
