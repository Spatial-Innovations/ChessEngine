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
    vector<Coords> knightMoves;
    string piece;

    for(auto row = 0; row < _board.size(); row++) {
        for(auto col = 0; col < _board[row].size(); col++) {
            piece = _GetSymbol(_board[row][col]);
            if      (piece == "R" || piece == "Q") {
                // Top
                for (auto r = row; r > 0; r--) {
                    if (_board[r][col] > 6) {break;}
                    moves.push_back(Coords(r, col));
                    if (_board[r][col] > 0 && _board[r][col] <= 6) {break;}
                }

                // Bottom
                for (auto r = row; r < _board.size(); r++) {
                    if (_board[r][col] > 6) {break;}
                    moves.push_back(Coords(r, col));
                    if (_board[r][col] > 0 && _board[r][col] <= 6) {break;}
                }

                // Left
                for (auto c = col; c > 0; c--) {
                    if (_board[row][c] > 6) {break;}
                    moves.push_back(Coords(row, c));
                    if (_board[row][c] > 0 && _board[row][c] <= 6) {break;}
                }

                // Right
                for (auto c = col; c < _board.size(); c++) {
                    if (_board[row][c] > 6) {break;}
                    moves.push_back(Coords(row, c));
                    if (_board[row][c] > 0 && _board[row][c] <= 6) {break;}
                }
            }
            else if (piece == "r" || piece == "q") {
                // Top
                for (auto r = row; r > 0; r--) {
                    if (_board[r][col] > 0 && _board[r][col] <= 6) {break;}
                    moves.push_back(Coords(r, col));
                    if (_board[r][col] > 6) {break;}
                }

                // Bottom
                for (auto r = row; r < _board.size(); r++) {
                    if (_board[r][col] > 0 && _board[r][col] <= 6) {break;}
                    moves.push_back(Coords(r, col));
                    if (_board[r][col] > 6) {break;}
                }

                // Left
                for (auto c = col; c > 0; c--) {
                    if (_board[row][c] > 0 && _board[row][c] <= 6) {break;}
                    moves.push_back(Coords(row, c));
                    if (_board[row][c] > 6) {break;}
                }

                // Right
                for (auto c = col; c < _board.size(); c++) {
                    if (_board[row][c] > 0 && _board[row][c] <= 6) {break;}
                    moves.push_back(Coords(row, c));
                    if (_board[row][c] > 6) {break;}
                }
            }
            else if (piece == "N" || piece == "n") {
                knightMoves = {
                    Coords(row-2, col-1), Coords(row-2, col+1)
                };
            }
            else if (piece == "B" || piece == "Q") {
                // Top Left
                for (auto r = row; r > 0; r--) {
                    if (0 <= col - abs(r - row) && col - abs(r - row) <= _board.size()){
                        if (_board[r][col - abs(r - row)] > 6) {break;}
                        moves.push_back(Coords(r, col - abs(r - row)));
                        if (_board[r][col] > 0 && _board[r][col - abs(r - row)] <= 6) {break;}
                    }
                }

                // Top Right
                for (auto r = row; r > 0; r--) {
                    if (0 <= col + abs(r - row) && col + abs(r - row) <= _board.size()){
                        if (_board[r][col + abs(r - row)] > 6) {break;}
                        moves.push_back(Coords(r, col + abs(r - row)));
                        if (_board[r][col] > 0 && _board[r][col + abs(r - row)] <= 6) {break;}
                    }
                }

                // Bottom Left
                for (auto r = row; r < _board.size(); r++) {
                    if (0 <= col - abs(r - row) && col - abs(r - row) <= _board.size()){
                        if (_board[r][col - abs(r - row)] > 6) {break;}
                        moves.push_back(Coords(r, col - abs(r - row)));
                        if (_board[r][col] > 0 && _board[r][col - abs(r - row)] <= 6) {break;}
                    }
                }

                // Bottom Right
                for (auto r = row; r < _board.size(); r++) {
                    if (0 <= col + abs(r - row) && col + abs(r - row) <= _board.size()){
                        if (_board[r][col + abs(r - row)] > 6) {break;}
                        moves.push_back(Coords(r, col + abs(r - row)));
                        if (_board[r][col] > 0 && _board[r][col + abs(r - row)] <= 6) {break;}
                    }
                }
            }
            else if (piece == "b" || piece == "q") {
                // Top Left
                for (auto r = row; r > 0; r--) {
                    if (0 <= col - abs(r - row) && col - abs(r - row) <= _board.size()){
                        if (_board[r][col - abs(r - row)] > 0 && _board[r][col - abs(r - row)] <= 6) {break;}
                        moves.push_back(Coords(r, col - abs(r - row)));
                        if (_board[r][col - abs(r - row)] > 6) {break;}
                    }
                }

                // Top Right
                for (auto r = row; r > 0; r--) {
                    if (0 <= col + abs(r - row) && col + abs(r - row) <= _board.size()){
                        if (_board[r][col + abs(r - row)] > 0 && _board[r][col + abs(r - row)] <= 6) {break;}
                        moves.push_back(Coords(r, col + abs(r - row)));
                        if (_board[r][col + abs(r - row)] > 6) {break;}
                    }
                }

                // Bottom Left
                for (auto r = row; r < _board.size(); r++) {
                    if (0 <= col - abs(r - row) && col - abs(r - row) <= _board.size()){
                        if (_board[r][col - abs(r - row)] > 0 && _board[r][col - abs(r - row)] <= 6) {break;}
                        moves.push_back(Coords(r, col - abs(r - row)));
                        if (_board[r][col - abs(r - row)] > 6) {break;}
                    }
                }

                // Bottom Right
                for (auto r = row; r < _board.size(); r++) {
                    if (0 <= col + abs(r - row) && col + abs(r - row) <= _board.size()){
                        if (_board[r][col + abs(r - row)] > 0 && _board[r][col + abs(r - row)] <= 6) {break;}
                        moves.push_back(Coords(r, col + abs(r - row)));
                        if (_board[r][col + abs(r - row)] > 6) {break;}
                    }
                }
            }
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