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


bool Startswith(string str, string start) {
    return (str.substr(0, start.size()) == start);
}


bool Endswith(string str, string end) {
    return (str.substr(str.size()-end.size(), end.size()) == end);
}


string Replace(string str, string substr, string replaceStr) {
    string final = "";
    int index;

    while (true) {
        index = str.find(substr);
        if (index == -1) {break;}

        final += str.substr(0, index);
        final += replaceStr;
        str = str.substr(index+substr.size(), str.size());
    }

    return final;
}


string Strip(string str) {
    int begInd, endInd;
    begInd = str.find_first_not_of(" ");
    endInd = str.find_last_not_of(" ");
    if (begInd == -1) {begInd = 0;}
    if (endInd == -1) {endInd = str.size();}

    return str.substr(begInd, endInd-begInd+1);
}


vector<string> Split(string str) {
    vector<string> parts = {};
    string currString = "";

    for (auto i = 0; i < str.size(); i++) {
        if (str.substr(i, 1) == " ") {
            parts.push_back(currString);
            currString = "";
        } else {
            currString += str.substr(i, 1);
        }
    }
    if (currString.size() > 0) {parts.push_back(currString);}

    return parts;
}