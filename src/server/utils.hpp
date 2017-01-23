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


template<typename T>
constexpr T min(T a, T b)
{
    return (a < b) ? a : b;
}

template<typename T>
constexpr T max(T a, T b)
{
    return (a > b) ? a : b;
}

template<typename T>
constexpr T enbounds(T a, T mini, T maxi)
{
    return min(max(a, mini), maxi);
}

template<typename T>
constexpr T sgn(T a)
{
    return (a < 0) ? T(-1) : T(1);
}

constexpr float mod_theta(float theta)
{
    while (theta <= 0)
    {
        theta += 360;
    }
    while (theta > 360)
    {
        theta -= 360;
    }

    if (theta > 180)
    {
        theta -= 360;
    }

    return theta;
}
