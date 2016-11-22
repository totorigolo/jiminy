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

#include <iomanip>
#include <iterator>
#include <iostream>
#include <algorithm>

#include "DebugTools.h"


int FunctionReporter::scope_counter = 0;

FunctionReporter::FunctionReporter(const char *functionName)
        : mName(functionName)
{
    std::cout << '[' << std::setw(3) << scope_counter + 1 << "] ";
    std::generate_n(std::ostream_iterator<char>(std::cout, ""), scope_counter, []()
    { return ' '; });
    std::cout << "---- " << "Beginning of #" << mName << " ----\n";
    ++scope_counter;
}

FunctionReporter::~FunctionReporter()
{
    --scope_counter;
    std::cout << '[' << std::setw(3) << scope_counter + 1 << "] ";
    std::generate_n(std::ostream_iterator<char>(std::cout, ""), scope_counter, []()
    { return ' '; });
    std::cout << "---- " << "End of #" << mName << " ----\n";
}
