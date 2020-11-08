//  ##### BEGIN GPL LICENSE BLOCK #####
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
//  ##### END GPL LICENSE BLOCK #####

#include "move.hpp"

Move::~Move() {
}


Move::Move(string first, string second) {
    _first = first;
    _second = second;
}


string Move::GetFirst(void) {
    return _first;
}


string Move::GetSecond(void) {
    return _second;
}


string Move::_GetSquare(int row, int col) {
    string strRow, strCol;

    switch (row) {
        case 0: strRow = "8"; break;
        case 1: strRow = "7"; break;
        case 2: strRow = "6"; break;
        case 3: strRow = "5"; break;
        case 4: strRow = "4"; break;
        case 5: strRow = "3"; break;
        case 6: strRow = "2"; break;
        case 7: strRow = "1"; break;
        default: break;
    }
    switch (col) {
        case 0: strCol = "a"; break;
        case 1: strCol = "b"; break;
        case 2: strCol = "c"; break;
        case 3: strCol = "d"; break;
        case 4: strCol = "e"; break;
        case 5: strCol = "f"; break;
        case 6: strCol = "g"; break;
        case 7: strCol = "h"; break;
    }

    return (strCol + strRow);
}