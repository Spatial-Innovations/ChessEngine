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


Board::~Board() {}


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

    _moves = {};
    _legalCastles = {true, true, true, true};
    _turn = true;
    _epSquare = {};
    _ep = false;
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
    printStr += "\nFen: ";
    printStr += GetFen();
    cout << printStr << endl;
}


void Board::SetMoves(vector<Move> moves) {_moves = moves;}
void Board::Push(Move move) {_moves.push_back(move);}


vector<Move> Board::GetMoves(void) {return _moves;}

vector<Move> Board::GetLegalMoves(void) {
    vector<Move> moves;
    return moves;
}


string Board::GetFen(void) {
    string fen;
    int slashCount = 1;
    int spaceCount = 0;

    for (auto row = 0; row < _board.size(); row++) {
        for (auto col = 0; col < _board[row].size(); col++) {
            if (row == slashCount) {
                if (spaceCount != 0) {
                    fen += to_string(spaceCount);
                    spaceCount = 0;
                }
                fen += "/";
                slashCount++;
            }
            if (_board[row][col] == 0) {spaceCount++;}
            else {
                if (spaceCount != 0) {
                    fen += to_string(spaceCount);
                    spaceCount = 0;
                }
                fen += _GetSymbol(_board[row][col]);
            }
        }
    }

    if (_turn) {fen += " w ";}
    else {fen += " b ";}

    for (auto i = 0; i < 4; i++) {
        if (_legalCastles[i]) {fen += _castleSymbols[i];}
    }

    fen += " ";
    if (_ep) {fen += _GetSquare(_epSquare[0], _epSquare[1]);}
    else {fen += "-";}
    fen += " 0 1";
    return fen;
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


string Board::_GetSquare(int row, int col) {
    string strRow, strCol;

    switch (row) {
        case 0: strRow = "8"; break;
        case 1: strRow = "7"; break;
        case 2: strRow = "6"; break;
        case 3: strRow = "5"; break;
        case 4: strRow = "4"; break;
        case 5: strRow = "3"; break;
        case 6: strRow = "2"; break;
        case 7: strRow = "1"; break;
        default: break;
    }
    switch (col) {
        case 0: strCol = "a"; break;
        case 1: strCol = "b"; break;
        case 2: strCol = "c"; break;
        case 3: strCol = "d"; break;
        case 4: strCol = "e"; break;
        case 5: strCol = "f"; break;
        case 6: strCol = "g"; break;
        case 7: strCol = "h"; break;
    }

    return (strCol + strRow);
}


vector<Move> Board::_GetKingMoves(vector<int> pieceLoc, bool color) {
    int row, col, currSquare;
    vector<vector<int>> locations;
    vector<int> currLoc;
    vector<Move> moves;

    row = pieceLoc[0];
    col = pieceLoc[1];
    locations = {
        {col - 1, row - 1}, {col - 1, row}, {col - 1, row + 1},
        {col, row - 1}, {col, row + 1},
        {col + 1, row - 1}, {col + 1, row}, {col + 1, row + 1}
    };

    for (auto i = 0; i < locations.size(); i++) {
        currLoc = {locations[i][1], locations[i][0]};
        currSquare = _board[currLoc[0]][currLoc[1]];
        if (color && (currSquare==0 || (currSquare >= 1 && currSquare <= 6))) {
            moves.push_back(Move(pieceLoc, currLoc));
        } else if (color==false && (currSquare==0 || currSquare >= 7 && currSquare <= 12)) {
            moves.push_back(Move(pieceLoc, currLoc));
        }
    }
}