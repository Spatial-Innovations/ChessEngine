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
#include "move.hpp"
using namespace std;


Move::~Move() {}
Move::Move() {}
Move::Move(vector<int> sq1, vector<int> sq2, string promo) {
    _square1 = sq1;
    _square2 = sq2;
    _promoPiece = promo;
    _promo = (promo != "");
}


void Move::SetSq1(vector<int> square) {_square1 = square;}
void Move::SetSq2(vector<int> square) {_square2 = square;}
void Move::SetPromotion(string promo) {
    _promoPiece = promo;
    _promo = (promo != "");
}


vector<int> Move::GetSq1(void) {return _square1;}
vector<int> Move::GetSq2(void) {return _square2;}
string Move::GetPromoPiece(void) {return _promoPiece;}
bool Move::GetPromo(void) {return _promo;}