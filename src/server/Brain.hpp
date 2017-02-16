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

#include <functional>
#include <string>
#include <list>
#include <map>

#include <mlpack/core.hpp>
#include <armadillo>


class Brain
{

public:
    Brain();

    virtual ~Brain();

    virtual void Think() final;

    virtual void Save() const final;

    virtual bool Failed() const final;

    virtual void Reseted() final;

protected:
    virtual void onThink() = 0;
    virtual void onSave() const = 0;
    virtual void onReseted() = 0;

    // TODO: Make mActions private
public:
    std::map<std::string, std::function<float(void)>> mInfo;
    std::map<std::string, std::function<void(float)>> mActions;
protected:
    unsigned long long int mIteration;
    bool mSimulated;
    bool mFailed;
};
