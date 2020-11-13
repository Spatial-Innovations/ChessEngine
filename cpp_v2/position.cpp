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

#include <iostream>
#include <vector>
#include <string>
#include "position.hpp"
using namespace std;


Position::~Position() {}


Position::Position() {
    _position = {
        {10, 8, 9, 11, 12, 9, 8, 10},
        {7,  7, 7, 7,  7,  7, 7, 7},
        {0,  0, 0, 0,  0,  0, 0, 0},
        {0,  0, 0, 0,  0,  0, 0, 0},
        {0,  0, 0, 0,  0,  0, 0, 0},
        {0,  0, 0, 0,  0,  0, 0, 0},
        {1,  1, 1, 1,  1,  1, 1, 1},
        {4,  2, 3, 5,  6,  3, 2, 4}
    };
}


string Position::_ToUci(int row, int col) {
    string uci = "";
    
    switch (col) {
        case 0: uci += "a"; break;
        case 1: uci += "b"; break;
        case 2: uci += "c"; break;
        case 3: uci += "d"; break;
        case 4: uci += "e"; break;
        case 5: uci += "f"; break;
        case 6: uci += "g"; break;
        case 7: uci += "h"; break;
        default: break;
    } switch (row) {
        case 0: uci += "8"; break;
        case 1: uci += "7"; break;
        case 2: uci += "6"; break;
        case 3: uci += "5"; break;
        case 4: uci += "4"; break;
        case 5: uci += "3"; break;
        case 6: uci += "2"; break;
        case 7: uci += "1"; break;
        default: break;
    }

    return uci;
}


vector<int> Position::_ToSquare(string uci) {
    
}