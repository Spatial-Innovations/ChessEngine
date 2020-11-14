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


class Move {
    public:
        ~Move();
        Move();
        Move(vector<int>, vector<int>, string="");

        void SetSq1(vector<int>);
        void SetSq2(vector<int>);
        void SetPromotion(string);

        vector<int> GetSq1(void);
        vector<int> GetSq2(void);
        string GetPromoPiece(void);
        bool GetPromo(void);

    private:
        vector<int> _square1, _square2;
        bool _promo;
        string _promoPiece;
};