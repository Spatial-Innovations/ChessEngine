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
#include "eval.hpp"
using namespace std;


int Eval(vector<vector<int>> position) {
    return Material(position);
}


int Material(vector<vector<int>> position) {
    int material = 0;

    for (auto row: position) {
        for (auto piece: row) {
            switch (piece) {
                case 1: material += 1; break;
                case 2: material += 3; break;
                case 3: material += 3; break;
                case 4: material += 5; break;
                case 5: material += 9; break;
                case 7: material -= 1; break;
                case 8: material -= 3; break;
                case 9: material -= 3; break;
                case 10: material -= 5; break;
                case 11: material -= 9; break;
            }
        }
    }
    
    return material;
}