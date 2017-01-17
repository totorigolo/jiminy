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


template<class S>
class Singleton
{
public:
    static S &Instance()
    {
        static S myInstance; // thread safe in C++11
        return myInstance;
    }

    // delete copy and move constructors and assign operators
    Singleton(S const &) = delete;             // Copy construct
    Singleton(S &&) = delete;                  // Move construct
    Singleton &operator=(S const &) = delete;  // Copy assign
    Singleton &operator=(S &&) = delete;      // Move assign

protected:
    Singleton()
    {}

    virtual ~Singleton()
    {}
};
