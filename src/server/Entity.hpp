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

#include <vector>
#include <memory>

#include <SFML/Graphics/Color.hpp>

#include "Brain.hpp"
#include "Limb.hpp"


class Entity
{
public:
    Entity();

    Entity(Entity const &) = delete;

    Entity &operator=(Entity const &) = delete;

    virtual ~Entity() = default;

    template<typename ... Args>
    std::shared_ptr<Limb> CreateLimb(Args &&... args)
    {
        mLimbs.emplace_back(std::make_shared<Limb>(std::forward<Args>(args)...));
        return mLimbs.back();
    }

    void Think();

    // TODO: make mBrain private
    Brain mBrain;

private:
    sf::Color mColor; // = owner ?
    std::vector<std::shared_ptr<Limb>> mLimbs;
};
