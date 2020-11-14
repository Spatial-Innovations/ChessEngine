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
#include "funcs.hpp"
using namespace std;


string Strip(string str) {
    string newStr = str;
    int begInd = 0, endInd = newStr.size() + 1;

    for (auto i = 0; i < str.size(); i++) {
        begInd = i;
        if (newStr.substr(i, 1) != " " && newStr.substr(i, 1) != "\n") {break;}
    }
    for (auto i = newStr.size()-1; i >= 0; i--) {
        if (newStr.substr(i, 1) != " " && newStr.substr(i, 1) != "\n") {break;}
        endInd = i;
    }

    return newStr.substr(begInd, endInd-begInd);
}