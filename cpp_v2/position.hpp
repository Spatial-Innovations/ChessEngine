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

#pragma once
#include <iostream>
#include <vector>
#include <string>
using namespace std;


class Position {
    // empty=0, wpawn=1, wknight=2, wbishop=3, wrook=4, wqueen=5, wking=6, bpawn=7, bknight=8, bbishop=9, brook=10, bqueen=11, bking=12
    public:
        ~Position();
        Position();

        void Push(vector<int>, vector<int>);

    private:
        vector<vector<int>> _position;

        string _ToUci(int, int);
        vector<int> _ToCoords(string);
};