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
#include "board.hpp"
using namespace std;

Board::~Board() {
}

Board::Board() {
    _board = {
        {4, 2, 3, 5, 6, 3, 2, 4},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {7, 7, 7, 7, 7, 7, 7, 7},
        {10, 8, 9, 11, 12, 9, 8, 10}
    };
}

void Board::Print(void) {
    string printStr = "", currChar;
    for (auto row = 0; row < 8; row++) {
        printStr += _printLine;
        printStr += _printSplit;
        for (auto col = 0; col < 8; col++) {
            currChar = _GetSymbol(_board[row][col]);
            printStr += currChar;
            printStr += _printSplit;
        }
        printStr += "\n";
    }

    printStr += _printLine;
    cout << printStr;
}

vector<Move> Board::GetLegalMoves(void) {
    vector<Move> moves;
    string piece;

    for(auto row = 0; row < _board.size(); row++) {
        for(auto col = 0; col < _board[row].size(); col++) {
            piece = _GetSymbol(_board[row][col]);
        }
    }

    return moves;
}

string Board::_GetSymbol(int num) {
    switch (num) {
    case 0: return " ";
    case 1: return "p";
    case 2: return "n";
    case 3: return "b";
    case 4: return "r";
    case 5: return "q";
    case 6: return "k";
    case 7: return "P";
    case 8: return "N";
    case 9: return "B";
    case 10: return "R";
    case 11: return "Q";
    case 12: return "K";
    default: return "";
    }
}