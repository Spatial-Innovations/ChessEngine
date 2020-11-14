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
    if (str.substr(0, start.size()) == start) {return true;}
    else {return false;}
}


bool Endswith(string str, string end) {
    if (str.substr(str.size()-end.size(), end.size()) == end) {return true;}
    else {return false;}
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


vector<string> Split(string str, string split) {
    vector<string> parts;
    int startInd = 0;

    for (auto i = 0; i < str.size()-split.size()+1; i++) {
        if (str.substr(i, split.size()) == split) {
            parts.push_back(str.substr(startInd, i-startInd));
            startInd = i + split.size();
        }
    }
    
    parts.push_back(str.substr(startInd, split.size()-startInd));
    return parts;
}