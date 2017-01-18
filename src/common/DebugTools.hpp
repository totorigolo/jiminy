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

#ifdef MAP
#define REPORT_BEGIN std::cout << BLE "## Beggining of " << __PRETTY_FUNCTION__ << " ##" RESET << std::endl
#define REPORT_END std::cout << JAU "## End of " << __PRETTY_FUNCTION__ << " ##" RESET << std::endl

#ifdef MAP_EXTRA
#define REPORT_BEGIN_EXTRA IF_MAP_DECLARER_DEBUT
#define REPORT_END_EXTRA IF_MAP_DECLARER_FIN
#else
#define REPORT_BEGIN_EXTRA ;
#define REPORT_END_EXTRA ;
#endif

#else
#define REPORT_BEGIN ;
#define REPORT_END ;
#define REPORT_BEGIN_EXTRA ;
#define REPORT_END_EXTRA ;
#endif
