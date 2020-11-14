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
#include "position.hpp"
#include "move.hpp"
using namespace std;


Position::~Position() {}


Position::Position() {
    Reset();
}


void Position::Reset(void) {
    _position = {
        {10, 8, 9, 11, 12, 9, 8, 10},
        {7 , 7, 7, 7 , 7 , 7, 7, 7 },
        {0 , 0, 0, 0 , 0 , 0, 0, 0 },
        {0 , 0, 0, 0 , 0 , 0, 0, 0 },
        {0 , 0, 0, 0 , 0 , 0, 0, 0 },
        {0 , 0, 0, 0 , 0 , 0, 0, 0 },
        {1 , 1, 1, 1 , 1 , 1, 1, 1 },
        {4 , 2, 3, 5 , 6 , 3, 2, 4 }
    };
    _moveStack.clear();
    _branches.clear();
    _turn = true;
    _castleRights = {true, true, true, true};
    _epLegal = false;
    _epSquare.clear();
}


void Position::Print(void) {
    string printStr = "";
    string const lineSep = " +---+---+---+---+---+---+---+---+\n";
    string const colSep = " | ";
    printStr += lineSep;

    for (auto row = 0; row < _position.size(); row++) {
        printStr += colSep;
        for (auto col = 0; col < _position[0].size(); col++) {
            printStr += _IntToPiece(_position[row][col]);
            printStr += colSep;
        }
        printStr += to_string(8-row) + "\n";
        printStr += lineSep;
    }
    printStr += "   a   b   c   d   e   f   g   h\n\nFen: ";
    printStr += GetFen();

    cout << printStr << endl;
}


vector<Move> Position::GetMoveStack(void) {
    return _moveStack;
}


int Position::GetNumNodes(void) {
    return 1 + _branches.size();
}


string Position::GetFen(void) {
    string fen = "";
    int piece, spaceCount = 0;

    for (auto row = 0; row < _position.size(); row++) {
        if (row != 0) {fen += "/";}
        for (auto col = 0; col < _position[0].size(); col++) {
            piece = _position[row][col];
            if (piece >= 1 && piece <= 12) {
                if (spaceCount > 0) {
                    fen += to_string(spaceCount);
                    spaceCount = 0;
                }
                fen += _IntToPiece(piece);
            } else if (piece == 0) {
                spaceCount += 1;
            }
        }
        if (spaceCount > 0) {
            fen += to_string(spaceCount);
            spaceCount = 0;
        }
    }

    if (_turn) {fen += " w ";}
    else {fen += " b ";}

    if (_castleRights[0] || _castleRights[1] || _castleRights[2] || _castleRights[3]) {
        if (_castleRights[0]) {fen += "K";}
        if (_castleRights[1]) {fen += "Q";}
        if (_castleRights[2]) {fen += "k";}
        if (_castleRights[3]) {fen += "q";}
    } else {
        fen += "-";
    }
    fen += " ";

    if (_epLegal) {fen += _CoordsToSquare(_epSquare);}
    else {fen += "-";}
    fen += " 0 1";

    return fen;
}


int Position::_PieceToInt(string piece) {
    int num;
    
    if (piece == " ") {num = 0;}
    else if (piece == "P") {num = 1;}
    else if (piece == "N") {num = 2;}
    else if (piece == "B") {num = 3;}
    else if (piece == "R") {num = 4;}
    else if (piece == "Q") {num = 5;}
    else if (piece == "K") {num = 6;}
    else if (piece == "p") {num = 7;}
    else if (piece == "n") {num = 8;}
    else if (piece == "b") {num = 9;}
    else if (piece == "r") {num = 0;}
    else if (piece == "q") {num = 1;}
    else if (piece == "k") {num = 2;}
    
    return num;
}


string Position::_IntToPiece(int piece) {
    switch (piece) {
        case 0: return " ";
        case 1: return "P";
        case 2: return "N";
        case 3: return "B";
        case 4: return "R";
        case 5: return "Q";
        case 6: return "K";
        case 7: return "p";
        case 8: return "n";
        case 9: return "b";
        case 10: return "r";
        case 11: return "q";
        case 12: return "k";
        default: return "";
    }
}


string Position::_CoordsToSquare(vector<int> coords) {
    // takes {row, col}
    string uci = "";
    int col, row;
    col = coords[1];
    row = coords[0];

    switch (col) {
        case 0: uci += "a"; break;
        case 1: uci += "b"; break;
        case 2: uci += "c"; break;
        case 3: uci += "d"; break;
        case 4: uci += "e"; break;
        case 5: uci += "f"; break;
        case 6: uci += "g"; break;
        case 7: uci += "h"; break;
        default: break;
    } switch (row) {
        case 0: uci += "8"; break;
        case 1: uci += "7"; break;
        case 2: uci += "6"; break;
        case 3: uci += "5"; break;
        case 4: uci += "4"; break;
        case 5: uci += "3"; break;
        case 6: uci += "2"; break;
        case 7: uci += "1"; break;
        default: break;
    }

    return uci;
}


vector<int> Position::_SquareToCoords(string uci) {
    // returns {row, col}
    vector<int> coords;
    string char1 = uci.substr(0, 1), char2 = uci.substr(1, 1);

    if (char2 == "1") {coords.push_back(7);}
    else if (char2 == "2") {coords.push_back(6);}
    else if (char2 == "3") {coords.push_back(5);}
    else if (char2 == "4") {coords.push_back(4);}
    else if (char2 == "5") {coords.push_back(3);}
    else if (char2 == "6") {coords.push_back(2);}
    else if (char2 == "7") {coords.push_back(1);}
    else if (char2 == "8") {coords.push_back(0);}

    if (char1 == "a") {coords.push_back(0);}
    else if (char1 == "b") {coords.push_back(1);}
    else if (char1 == "c") {coords.push_back(2);}
    else if (char1 == "d") {coords.push_back(3);}
    else if (char1 == "e") {coords.push_back(4);}
    else if (char1 == "f") {coords.push_back(5);}
    else if (char1 == "g") {coords.push_back(6);}
    else if (char1 == "h") {coords.push_back(7);}

    return coords;
}


Move Position::_UciToMove(string uci) {
    vector<int> square1, square2;
    string promo;
    
    square1 = _SquareToCoords(uci.substr(0, 2));
    square2 = _SquareToCoords(uci.substr(2, 2));
    promo = uci.substr(4, 1);

    return Move(square1, square2, promo);
}


string Position::_MoveToUci(Move move) {
    string square1, square2, promo;

    square1 = _CoordsToSquare(move.GetSq1());
    square2 = _CoordsToSquare(move.GetSq2());
    promo = move.GetPromoPiece();

    return square1 + square2 + promo;
}


vector<int> Position::_GetKingPos(bool color) {
    vector<int> coords;

    if (color) {
        for (auto row = 0; row < _position.size(); row++) {
            for (auto col = 0; col < _position[0].size(); col++) {
                if (_position[row][col] == 6) {coords = {row, col};}
            }
        }
    } else {
        for (auto row = 0; row < _position.size(); row++) {
            for (auto col = 0; col < _position[0].size(); col++) {
                if (_position[row][col] == 12) {coords = {row, col};}
            }
        }
    }

    return coords;
}