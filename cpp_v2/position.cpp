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


void Position::Push(Move move) {
    vector<int> square1, square2;
    string promo;

    square1 = move.GetSq1();
    square2 = move.GetSq2();
    promo = move.GetPromoPiece();

    if (move.GetPromo()) {_position[square2[0]][square2[1]] = _PieceToInt(promo);}
    else {_position[square2[0]][square2[1]] = _position[square1[0]][square2[1]];}
    _position[square1[0]][square2[1]] = 0;
    _turn = !_turn;
}


void Position::PushUci(string uci) {
    Move move = _UciToMove(uci);
    Push(move);
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


vector<Move> Position::GetMoveStack(void) {
    return _moveStack;
}


vector<Move> Position::GetLegalMoves(void) {
    vector<Move> moves, currMoves;
    int piece;

    for (auto row = 0; row < _position.size(); row++) {
        for (auto col = 0; col < _position[0].size(); col++) {
            piece = _position[row][col];
            
            if (piece == 0) {continue;}
            if (_turn && piece >= 7 && piece <= 12) {continue;}
            if (!_turn && piece >= 1 && piece <= 6) {continue;}
            
            currMoves.clear();
            switch (piece) {
                //case 1: currMoves = _GetPawnMoves({row, col}, true); break;
                //case 2: currMoves = _GetKnightMoves({row, col}, true); break;
                case 3: currMoves = _GetBishopMoves({row, col}, true); break;
                //case 4: currMoves = _GetRookMoves({row, col}, true); break;
                //case 6: currMoves = _GetKingMoves({row, col}, true); break;
                //case 7: currMoves = _GetPawnMoves({row, col}, false); break;
                //case 8: currMoves = _GetKnightMoves({row, col}, false); break;
                case 9: currMoves = _GetBishopMoves({row, col}, false); break;
                //case 10: currMoves = _GetRookMoves({row, col}, false); break;
                //case 12: currMoves = _GetKingMoves({row, col}, false); break;
                default: break;
            }

            for (auto move: currMoves) {
                moves.push_back(move);
            }
        }
    }

    return moves;
}


void Position::SetFen(string fen) {
    vector<string> parts;
    string currChar;

    parts = Split(Strip(fen));

    _position.clear();
    _position.push_back({});
    for (auto i = 0; i < parts[0].size(); i++) {
        currChar = parts[0].substr(i, 1);

        if (currChar == "/") {
            _position.push_back({});
        } else if (currChar == "1" || currChar == "2" || currChar == "3" || currChar == "4" || currChar == "5" || currChar == "6" || currChar == "7" || currChar == "8") {
            for (auto j = 0; j < stoi(currChar); j++) {_position[_position.size()-1].push_back(0);}
        } else {
            _position[_position.size()-1].push_back(_PieceToInt(currChar));
        }
    }

    if (parts[1] == "w") {_turn = true;}
    else {_turn = false;}

    if (parts[2] == "-") {_castleRights = {false, false, false, false};}
    else {
        for (auto i = 0; i < parts[2].size(); i++) {
            if (parts[2].substr(i, 1) == "K") {_castleRights[0] = true;}
            else if (parts[2].substr(i, 1) == "Q") {_castleRights[0] = true;}
            else if (parts[2].substr(i, 1) == "k") {_castleRights[0] = true;}
            else if (parts[2].substr(i, 1) == "q") {_castleRights[0] = true;}
        }
    }

    if (parts[3] == "-") {
        _epLegal = false;
        _epSquare.clear();
    } else {
        _epLegal = true;
        _epSquare = _SquareToCoords(parts[3]);
    }
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
    else if (piece == "r") {num = 10;}
    else if (piece == "q") {num = 11;}
    else if (piece == "k") {num = 12;}

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
    if (color) {
        for (auto row = 0; row < _position.size(); row++) {
            for (auto col = 0; col < _position[0].size(); col++) {
                if (_position[row][col] == 6) {return {row, col};}
            }
        }
    } else {
        for (auto row = 0; row < _position.size(); row++) {
            for (auto col = 0; col < _position[0].size(); col++) {
                if (_position[row][col] == 12) {return {row, col};}
            }
        }
    }

    return {-1, -1};
}


vector<Move> Position::_GetKingMoves(vector<int> coords, bool color) {
    vector<vector<int>> squares;
    vector<Move> moves;
    int row, col, piece;

    row = coords[0];
    col = coords[1];
    squares = {
        {row-1, col-1}, {row, col-1}, {row+1, col-1},
        {row-1, col}, {row+1, col},
        {row-1, col+1}, {row, col+1}, {row+1, col+1}
    };

    for (auto square: squares) {
        row = square[0];
        col = square[1];

        if (row >= 0 && row <= 7 && col >= 0 && col <= 7) {
            piece = _position[row][col];
            if (color && (piece == 0 || (piece >= 7 && piece <= 11))) {moves.push_back(Move(coords, square));}
            else if (!color && (piece == 0 || (piece >= 1 && piece <= 6))) {moves.push_back(Move(coords, square));}
        }
    }

    return moves;
}


vector<Move> Position::_GetKnightMoves(vector<int> coords, bool color) {
    vector<vector<int>> squares;
    vector<Move> moves;
    int row, col, piece;

    row = coords[0];
    col = coords[1];
    squares = {
        {row-2, col-1}, {row-2, col+1},
        {row-1, col-2}, {row-1, col+2},
        {row+1, col-2}, {row+1, col+2},
        {row+2, col-1}, {row+2, col+1},
    };

    for (auto square: squares) {
        row = square[0];
        col = square[1];

        if (row >= 0 && row <= 7 && col >= 0 && col <= 7) {
            piece = _position[row][col];
            if (color && (piece == 0 || (piece >= 7 && piece <= 11))) {moves.push_back(Move(coords, square));}
            else if (!color && (piece == 0 || (piece >= 1 && piece <= 6))) {moves.push_back(Move(coords, square));}
        }
    }

    return moves;
}


vector<Move> Position::_GetPawnMoves(vector<int> coords, bool color) {
    vector<Move> moves;
    vector<int> captureL, captureR;
    const vector<string> promoPieces = {"N", "B", "R", "Q"};
    int row, col, captureLPiece, captureRPiece;

    row = coords[0];
    col = coords[1];

    if (color) {
        // Forward moves
        if (row == 6) {
            if (_position[row-1][col] == 0) {moves.push_back(Move(coords, {row-1, col}));}
            if (_position[row-2][col] == 0) {moves.push_back(Move(coords, {row-2, col}));}
        } else if (row <= 5 && row >= 2) {
            if (_position[row-1][col] == 0) {moves.push_back(Move(coords, {row-1, col}));}
        } else if (row == 1) {
            for (auto promo: promoPieces) {moves.push_back(Move(coords, {row-1, col}, promo));}
        }

        // Captures
        captureL = {row-1, col-1};
        captureR = {row-1, col+1};
        if (captureL[0] >= 0 && captureL[0] <= 7 && captureL[1] >= 0 && captureL[1] <= 7) {
            captureLPiece = _position[captureL[0]][captureL[1]];
            if (captureLPiece >= 7 && captureLPiece <= 12) {
                if (row == 1) {
                    for (auto promo: promoPieces) {moves.push_back(Move(coords, captureL, promo));}
                } else {
                    moves.push_back(Move(coords, captureL));
                }
            }
        }
        if (captureR[0] >= 0 && captureR[0] <= 7 && captureR[1] >= 0 && captureR[1] <= 7) {
            captureRPiece = _position[captureR[0]][captureR[1]];
            if (captureRPiece >= 7 && captureRPiece <= 12) {
                if (row == 1) {
                    for (auto promo: promoPieces) {moves.push_back(Move(coords, captureR, promo));}
                } else {
                    moves.push_back(Move(coords, captureR));
                }
            }
        }

    } else {
        // Forward moves
        if (row == 1) {
            if (_position[row+1][col] == 0) {moves.push_back(Move(coords, {row+1, col}));}
            if (_position[row+2][col] == 0) {moves.push_back(Move(coords, {row+2, col}));}
        } else if (row <= 5 && row >= 2) {
            if (_position[row+1][col] == 0) {moves.push_back(Move(coords, {row+1, col}));}
        } else if (row == 6) {
            for (auto promo: promoPieces) {moves.push_back(Move(coords, {row+1, col}, promo));}
        }

        // Captures
        captureL = {row+1, col-1};
        captureR = {row+1, col+1};
        if (captureL[0] >= 0 && captureL[0] <= 7 && captureL[1] >= 0 && captureL[1] <= 7) {
            captureLPiece = _position[captureL[0]][captureL[1]];
            if (captureLPiece >= 1 && captureLPiece <= 6) {
                if (row == 6) {
                    for (auto promo: promoPieces) {moves.push_back(Move(coords, captureL, promo));}
                } else {
                    moves.push_back(Move(coords, captureL));
                }
            }
        }
        if (captureR[0] >= 0 && captureR[0] <= 7 && captureR[1] >= 0 && captureR[1] <= 7) {
            captureRPiece = _position[captureR[0]][captureR[1]];
            if (captureRPiece >= 1 && captureRPiece <= 6) {
                if (row == 6) {
                    for (auto promo: promoPieces) {moves.push_back(Move(coords, captureR, promo));}
                } else {
                    moves.push_back(Move(coords, captureR));
                }
            }
        }
    }

    return moves;
}


vector<Move> Position::_GetRookMoves(vector<int> coords, bool color) {
    vector<Move> moves;
    int row, col, currPiece;
    row = coords[0];
    col = coords[1];

    // Top
    for (auto r = row-1; r >= 0; r--) {
        currPiece = _position[r][col];
        if (currPiece == 0) {
            moves.push_back(Move(coords, {r, col}));
        } else if ((currPiece >= 1 && currPiece <= 6) && !color) {
            moves.push_back(Move(coords, {r, col}));
            break;
        } else if ((currPiece >= 7 && currPiece <= 12) && color) {
            moves.push_back(Move(coords, {r, col}));
            break;
        } else if (((currPiece >= 1 && currPiece <= 6) && color) || ((currPiece >= 7 && currPiece <= 12) && !color)) {
            break;
        }
    }

    // Bottom
    for (auto r = row+1; r <= 7; r++) {
        currPiece = _position[r][col];
        if (currPiece == 0) {
            moves.push_back(Move(coords, {r, col}));
        } else if ((currPiece >= 1 && currPiece <= 6) && !color) {
            moves.push_back(Move(coords, {r, col}));
            break;
        } else if ((currPiece >= 7 && currPiece <= 12) && color) {
            moves.push_back(Move(coords, {r, col}));
            break;
        } else if (((currPiece >= 1 && currPiece <= 6) && color) || ((currPiece >= 7 && currPiece <= 12) && !color)) {
            break;
        }
    }

    // Right
    for (auto c = col+1; c <= 7; c++) {
        currPiece = _position[row][c];
        if (currPiece == 0) {
            moves.push_back(Move(coords, {row, c}));
        } else if ((currPiece >= 1 && currPiece <= 6) && !color) {
            moves.push_back(Move(coords, {row, c}));
            break;
        } else if ((currPiece >= 7 && currPiece <= 12) && color) {
            moves.push_back(Move(coords, {row, c}));
            break;
        } else if (((currPiece >= 1 && currPiece <= 6) && color) || ((currPiece >= 7 && currPiece <= 12) && !color)) {
            break;
        }
    }

    // Left
    for (auto c = col-1; c >= 0; c--) {
        currPiece = _position[row][c];
        if (currPiece == 0) {
            moves.push_back(Move(coords, {row, c}));
        } else if ((currPiece >= 1 && currPiece <= 6) && !color) {
            moves.push_back(Move(coords, {row, c}));
            break;
        } else if ((currPiece >= 7 && currPiece <= 12) && color) {
            moves.push_back(Move(coords, {row, c}));
            break;
        } else if (((currPiece >= 1 && currPiece <= 6) && color) || ((currPiece >= 7 && currPiece <= 12) && !color)) {
            break;
        }
    }

    return moves;
}


vector<Move> Position::_GetBishopMoves(vector<int> coords, bool color) {
    vector<Move> moves;
    int row, col, currRow, currCol, currPiece, i;
    row = coords[0];
    col = coords[1];

    // Top left
    i = 1;
    while (true) {
        currRow = row - i;
        currCol = col - i;
        if (currRow < 0 || currCol < 0) {break;}
        currPiece = _position[currRow][currCol];

        if (currPiece == 0) {
            moves.push_back(Move(coords, {currRow, currCol}));
        } else if ((currPiece >= 1 && currPiece <= 6) && !color) {
            moves.push_back(Move(coords, {currRow, currCol}));
            break;
        } else if ((currPiece >= 7 && currPiece <= 12) && color) {
            moves.push_back(Move(coords, {currRow, currCol}));
            break;
        } else if (((currPiece >= 1 && currPiece <= 6) && color) || ((currPiece >= 7 && currPiece <= 12) && !color)) {
            break;
        }
        i++;
    }

    // Top right
    i = 1;
    while (true) {
        currRow = row - i;
        currCol = col + i;
        if (currRow < 0 || currCol < 0) {break;}
        currPiece = _position[currRow][currCol];

        if (currPiece == 0) {
            moves.push_back(Move(coords, {currRow, currCol}));
        } else if ((currPiece >= 1 && currPiece <= 6) && !color) {
            moves.push_back(Move(coords, {currRow, currCol}));
            break;
        } else if ((currPiece >= 7 && currPiece <= 12) && color) {
            moves.push_back(Move(coords, {currRow, currCol}));
            break;
        } else if (((currPiece >= 1 && currPiece <= 6) && color) || ((currPiece >= 7 && currPiece <= 12) && !color)) {
            break;
        }
        i++;
    }

    // Bottom left
    i = 1;
    while (true) {
        currRow = row + i;
        currCol = col - i;
        if (currRow < 0 || currCol < 0) {break;}
        currPiece = _position[currRow][currCol];

        if (currPiece == 0) {
            moves.push_back(Move(coords, {currRow, currCol}));
        } else if ((currPiece >= 1 && currPiece <= 6) && !color) {
            moves.push_back(Move(coords, {currRow, currCol}));
            break;
        } else if ((currPiece >= 7 && currPiece <= 12) && color) {
            moves.push_back(Move(coords, {currRow, currCol}));
            break;
        } else if (((currPiece >= 1 && currPiece <= 6) && color) || ((currPiece >= 7 && currPiece <= 12) && !color)) {
            break;
        }
        i++;
    }

    // Bottom right
    i = 1;
    while (true) {
        currRow = row + i;
        currCol = col + i;
        if (currRow < 0 || currCol < 0) {break;}
        currPiece = _position[currRow][currCol];

        if (currPiece == 0) {
            moves.push_back(Move(coords, {currRow, currCol}));
        } else if ((currPiece >= 1 && currPiece <= 6) && !color) {
            moves.push_back(Move(coords, {currRow, currCol}));
            break;
        } else if ((currPiece >= 7 && currPiece <= 12) && color) {
            moves.push_back(Move(coords, {currRow, currCol}));
            break;
        } else if (((currPiece >= 1 && currPiece <= 6) && color) || ((currPiece >= 7 && currPiece <= 12) && !color)) {
            break;
        }
        i++;
    }

    return moves;
}