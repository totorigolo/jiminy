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

#include <iostream>

//------------------------------------------------------------- Couleurs
#define RESET "\x1B[00m"
#define GRAS "\x1B[01m"
#define GRIS "\x1B[02m"
#define GRIS_F "\x1B[30m"
#define ITALIQUE "\x1B[03m"
#define SOULIGNE "\x1B[04m"
#define RGE "\x1B[31m"
#define VRT "\x1B[32m"
#define JAU "\x1B[33m"
#define BLE "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define BLC "\x1B[37m"
#define FOND_BLC "\x1B[07m"
#define FOND_RGE "\x1B[41m"
#define FOND_VRT "\x1B[42m"
#define FOND_JAU "\x1B[43m"
#define FOND_BLE "\x1B[44m"
#define FOND_MAG "\x1B[45m"
#define FOND_CYN "\x1B[46m"
#define FOND_GRIS "\x1B[47m"
#define FOND_GRIS_F "\x1B[40m"

// Make cin::operator>> safe to use with getline
#define SAFE_CIN(input) while (!(input)) \
    { \
        std::cin.clear(); \
        std::cout << RGE "Bad input, please try again: " RESET; \
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); \
    } \
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
