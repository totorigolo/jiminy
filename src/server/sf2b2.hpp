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

#include <SFML/System/Vector2.hpp>
#include <Box2D/Common/b2Math.h>


// Conversion : Meter <-> Pixel
constexpr int PPM = 100;
constexpr float32 MPP = 1 / float32(PPM);

inline sf::Vector2f convert(const b2Vec2 &b2Vec2)
{
    return {b2Vec2.x * PPM, -b2Vec2.y * PPM};
}

template <typename T>
inline b2Vec2 convert(const sf::Vector2<T> &sfVec2)
{
    return {float(sfVec2.x) * MPP, -float(sfVec2.y) * MPP};
}

inline sf::Color convert(const b2Color& color)
{
    return sf::Color(static_cast<sf::Uint8>(color.r * 255.f),
                     static_cast<sf::Uint8>(color.g * 255.f),
                     static_cast<sf::Uint8>(color.b * 255.f));
}

inline sf::Vector2f u2f(sf::Vector2u vec)
{
    return sf::Vector2f(static_cast<float>(vec.x), static_cast<float>(vec.y));
}
inline sf::Vector2i u2i(sf::Vector2u vec)
{
    return sf::Vector2i(static_cast<int>(vec.x), static_cast<int>(vec.y));
}
inline sf::Vector2u f2u(sf::Vector2f vec)
{
    return sf::Vector2u(static_cast<unsigned int>(vec.x), static_cast<unsigned int>(vec.y));
}
inline sf::Vector2i f2i(sf::Vector2f vec)
{
    return sf::Vector2i(static_cast<int>(vec.x), static_cast<int>(vec.y));
}
inline sf::Vector2f i2f(sf::Vector2i vec)
{
    return sf::Vector2f(static_cast<float>(vec.x), static_cast<float>(vec.y));
}
inline sf::Vector2u i2u(sf::Vector2i vec)
{
    return sf::Vector2u(static_cast<unsigned int>(vec.x), static_cast<unsigned int>(vec.y));
}

template<typename T>
sf::Vector2<T> operator*(sf::Vector2<T> a, sf::Vector2<T> b)
{
    return sf::Vector2<T>(a.x * b.x, a.y * b.y);
}
template<typename T>
sf::Vector2<T> operator/(sf::Vector2<T> a, sf::Vector2<T> b)
{
    return sf::Vector2<T>(a.x / b.x, a.y / b.y);
}
