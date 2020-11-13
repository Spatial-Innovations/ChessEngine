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
    _moves = {};
}


void Position::Push(vector<int> sq1, vector<int> sq2) {
    _position[sq2[0]][sq2[1]] = _position[sq1[0]][sq1[1]];
    _position[sq1[0]][sq1[1]] = 0;
    _moves.push_back({sq1, sq2});
}


vector<vector<vector<int>>> Position::GetMoves() {
    return _moves;
}


string Position::_CoordsToUci(vector<int> coords) {
    // takes {row, col}
    string uci = "";
    int col, row;
    col = coords[1];
    row = coords[0];
    
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


vector<int> Position::_UciToCoords(string uci) {
    // returns {row, col}
    vector<int> coords;
    string char1 = uci.substr(0, 1), char2 = uci.substr(1, 1);

    if (char2 == "1") {coords.push_back(7);}
    else if (char2 == "2") {coords.push_back(6);}
    else if (char2 == "3") {coords.push_back(5);}
    else if (char2 == "4") {coords.push_back(4);}
    else if (char2 == "5") {coords.push_back(3);}
    else if (char2 == "6") {coords.push_back(2);}
    else if (char2 == "7") {coords.push_back(1);}
    else if (char2 == "8") {coords.push_back(0);}

    if (char1 == "a") {coords.push_back(0);}
    else if (char1 == "b") {coords.push_back(1);}
    else if (char1 == "c") {coords.push_back(2);}
    else if (char1 == "d") {coords.push_back(3);}
    else if (char1 == "e") {coords.push_back(4);}
    else if (char1 == "f") {coords.push_back(5);}
    else if (char1 == "g") {coords.push_back(6);}
    else if (char1 == "h") {coords.push_back(7);}

    return coords;
}