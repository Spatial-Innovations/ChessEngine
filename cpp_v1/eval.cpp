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

#include <vector>
#include <string>
#include "eval.hpp"
using namespace std;


int Eval(vector<vector<int>> board) {
    return 1;
}


int Material(vector<vector<int>> board) {
    int points = 0;
    int piece;

    for (auto row = 0; row < board.size(); row++) {
        for (auto col = 0; col < board[row].size(); col++) {
            piece = board[row][col];
            if (piece == 1) {points -= 1;}
            else if (piece == 2) {points -= 3;}
            else if (piece == 3) {points -= 3;}
            else if (piece == 4) {points -= 5;}
            else if (piece == 5) {points -= 9;}
            else if (piece == 7) {points += 1;}
            else if (piece == 8) {points += 3;}
            else if (piece == 9) {points += 3;}
            else if (piece == 10) {points += 5;}
            else if (piece == 11) {points += 9;}
        }
    }

    return points;
}
