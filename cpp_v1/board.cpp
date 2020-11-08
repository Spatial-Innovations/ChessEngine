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
#include <cstdlib>
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
    return moves;
}


vector<Move> Board::_GetPawnMoves(vector<int> location, bool color) {
    vector<Move> moves;
    return moves;
}

vector<Move> Board::_GetKnightMoves(vector<int> location, bool color) {
    vector<Move> moves;
    return moves;
}

vector<Move> Board::_GetBishopMoves(vector<int> location, bool color) {
    vector<Move> moves;
    return moves;
}

vector<Move> Board::_GetRookMoves(vector<int> location, bool color) {
    vector<Move> moves;
    return moves;
}

vector<Move> Board::_GetQueenMoves(vector<int> location, bool color) {
    vector<Move> moves;
    return moves;
}

vector<Move> Board::_GetKingMoves(vector<int> location, bool color) {
    vector<Move> moves;
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


string Board::_GetSquare(vector<int> coords) {
    string row, col;

    switch (coords[0]) {
        case 0: row = "8"; break;
        case 1: row = "7"; break;
        case 2: row = "6"; break;
        case 3: row = "5"; break;
        case 4: row = "4"; break;
        case 5: row = "3"; break;
        case 6: row = "2"; break;
        case 7: row = "1"; break;
        default: break;
    }
    switch (coords[1]) {
        case 0: col = "a"; break;
        case 1: col = "b"; break;
        case 2: col = "c"; break;
        case 3: col = "d"; break;
        case 4: col = "e"; break;
        case 5: col = "f"; break;
        case 6: col = "g"; break;
        case 7: col = "h"; break;
    }

    return (col + row);
}