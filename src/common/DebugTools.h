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

#include <string>


class FunctionReporter
{
    static int scope_counter; // not thread aware

public:
    FunctionReporter(const char *functionName);

    virtual ~FunctionReporter();

private:
    std::string mName;
};

// TODO: We only want report in debug build environment
#define REPORT() FunctionReporter __decf_debug_info(__PRETTY_FUNCTION__)
