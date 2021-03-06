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
        {4 , 2, 3, 5 , 6 , 3, 2, 4 },
        {1 , 1, 1, 1 , 1 , 1, 1, 1 },
        {0 , 0, 0, 0 , 0 , 0, 0, 0 },
        {0 , 0, 0, 0 , 0 , 0, 0, 0 },
        {0 , 0, 0, 0 , 0 , 0, 0, 0 },
        {0 , 0, 0, 0 , 0 , 0, 0, 0 },
        {7 , 7, 7, 7 , 7 , 7, 7, 7 },
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


void Board::SetMoves(vector<Move> moves) {
    // todo edit board
    _moves = moves;
}


void Board::SetDepth(int depth) {
    _depth = depth;
}


void Board::Push(Move move) {
    // todo edit board and switch turn
    // todo update ep and castling
    _moves.push_back(move);
}


void Board::GenBranches(int targetDepth) {
    if (targetDepth == _depth + 1) {
        vector<Move> legalMoves = GetLegalMoves();
        vector<Move> currMoves = GetMoves();

        for (auto i = 0; i < legalMoves.size(); i++) {
            _branches.push_back(Board());
            _branches[i].SetDepth(_depth + 1);
            _branches[i].Push(legalMoves[i]);
        }
    }
    else if (targetDepth > _depth+1) {
        for (auto i = 0; i < _branches.size(); i++) {
            _branches[i].GenBranches(targetDepth);
        }
    }
}


void Board::ResetBranches(void) {
    // todo delete instead of set to none to save memory.
    _branches.clear();  //todo bug this line
}


vector<Move> Board::GetMoves(void) {
    return _moves;
}


vector<Move> Board::GetLegalMoves(void) {
    vector<Move> moves;
    vector<Move> currMoves;
    int piece;

    for (auto row = 0; row < _board.size(); row++) {
        for (auto col = 0; col < _board[row].size(); col++) {
            currMoves.clear();
            piece = _board[row][col];
            if (_turn) {
                switch (piece) {
                    case 7: currMoves = _GetPawnMoves({row, col}, true); break;  // todo pawn moves
                    case 8: currMoves = _GetKnightMoves({row, col}, true); break;
                    case 9: currMoves = _GetBishopMoves({row, col}, true); break;
                    case 10: currMoves = _GetRookMoves({row, col}, true); break;
                    case 11: currMoves = _GetQueenMoves({row, col}, true); break;
                    case 12: currMoves = _GetKingMoves({row, col}, true); break;
                    default: currMoves = {};
                }
            }
            else {
                switch (piece) {
                    case 1: currMoves = _GetPawnMoves({row, col}, false); break;  // todo pawn moves
                    case 2: currMoves = _GetKnightMoves({row, col}, false); break;
                    case 3: currMoves = _GetBishopMoves({row, col}, false); break;
                    case 4: currMoves = _GetRookMoves({row, col}, false); break;
                    case 5: currMoves = _GetQueenMoves({row, col}, false); break;
                    case 6: currMoves = _GetKingMoves({row, col}, false); break;
                    default: currMoves = {};
                }
            }

            for (auto i = 0; i < currMoves.size(); i++) {
                moves.push_back(currMoves[i]);
            }
        }
    }

    return moves;
}


bool Board::InCheck(void) {
    vector<Move> moves = GetLegalMoves();
    vector<int> kingPos = KingPos(_turn);

    for (auto i = 0; i < moves.size(); i++) {
        if (moves[i].GetPos()[1] == kingPos) {
            return true;
        }
    }

    return false;
}


vector<int> Board::KingPos(bool color) {
    for (auto row = 0; row < _board.size(); row++) {
        for (auto col = 0; col < _board[row].size(); col++) {
            if (color) {
                if (_board[row][col] == 12) {
                    return {row, col};
                }
            }
            else {
                if (_board[row][col] == 6) {
                    return {row, col};
                }
            }
        }
    }
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


vector<Move> Board::_GetPawnMoves(vector<int> location, bool color) {
    // todo
    vector<Move> moves;
    return moves;
}


vector<Move> Board::_GetKnightMoves(vector<int> location, bool color) {
    vector<Move> moves;
    int row = location[0];
    int col = location[1];
    int piece;

    vector<vector<int>> positions = {
        {row-2, col-1}, {row-2, col+1},
        {row-1, col-2}, {row-1, col+2},
        {row+2, col-1}, {row+2, col+1},
        {row+1, col-2}, {row+1, col+2}
    };

    for (auto i = 0; i < positions.size(); i++) {
        row = positions[i][0];
        col = positions[i][1];

        if (row >= 0 && col >= 0 && row <= 7 && col <= 7) {
            piece = _board[row][col];
            if (color) {
                if (piece == 0 || (piece >= 1 && piece <= 6)) {
                    moves.push_back(Move(location, {row, col}));
                }
            }
            else {
                if (piece == 0 || (piece >= 7 && piece <= 12)) {
                    moves.push_back(Move(location, {row, col}));
                }
            }
        }
    }

    return moves;
}


vector<Move> Board::_GetBishopMoves(vector<int> location, bool color) {
    vector<Move> moves;
    int row = location[0];
    int col = location[1];

    // White bishop
    if (color) {
        // Top Left
        for (auto r = row; r > 0; r--) {
            if (0 <= col - abs(r - row) && col - abs(r - row) <= _board.size()){
                if (_board[r][col - abs(r - row)] > 6) {break;}
                moves.push_back(Move(location, {r, col - abs(r - row)}));
                if (_board[r][col] > 0 && _board[r][col - abs(r - row)] <= 6) {break;}
            }
        }

        // Top Right
        for (auto r = row; r > 0; r--) {
            if (0 <= col + abs(r - row) && col + abs(r - row) <= _board.size()){
                if (_board[r][col + abs(r - row)] > 6) {break;}
                moves.push_back(Move(location, {r, col + abs(r - row)}));
                if (_board[r][col] > 0 && _board[r][col + abs(r - row)] <= 6) {break;}
            }
        }

        // Bottom Left
        for (auto r = row; r < _board.size(); r++) {
            if (0 <= col - abs(r - row) && col - abs(r - row) <= _board.size()){
                if (_board[r][col - abs(r - row)] > 6) {break;}
                moves.push_back(Move(location, {r, col - abs(r - row)}));
                if (_board[r][col] > 0 && _board[r][col - abs(r - row)] <= 6) {break;}
            }
        }

        // Bottom Right
        for (auto r = row; r < _board.size(); r++) {
            if (0 <= col + abs(r - row) && col + abs(r - row) <= _board.size()){
                if (_board[r][col + abs(r - row)] > 6) {break;}
                moves.push_back(Move(location, {r, col + abs(r - row)}));
                if (_board[r][col] > 0 && _board[r][col + abs(r - row)] <= 6) {break;}
            }
        }
    }

    // Black bishop
    else {
        // Top Left
        for (auto r = row; r > 0; r--) {
            if (0 <= col - abs(r - row) && col - abs(r - row) <= _board.size()){
                if (_board[r][col - abs(r - row)] > 0 && _board[r][col - abs(r - row)] <= 6) {break;}
                moves.push_back(Move(location, {r, col - abs(r - row)}));
                if (_board[r][col - abs(r - row)] > 6) {break;}
            }
        }

        // Top Right
        for (auto r = row; r > 0; r--) {
            if (0 <= col + abs(r - row) && col + abs(r - row) <= _board.size()){
                if (_board[r][col + abs(r - row)] > 0 && _board[r][col + abs(r - row)] <= 6) {break;}
                moves.push_back(Move(location, {r, col + abs(r - row)}));
                if (_board[r][col + abs(r - row)] > 6) {break;}
            }
        }

        // Bottom Left
        for (auto r = row; r < _board.size(); r++) {
            if (0 <= col - abs(r - row) && col - abs(r - row) <= _board.size()){
                if (_board[r][col - abs(r - row)] > 0 && _board[r][col - abs(r - row)] <= 6) {break;}
                moves.push_back(Move(location, {r, col - abs(r - row)}));
                if (_board[r][col - abs(r - row)] > 6) {break;}
            }
        }

        // Bottom Right
        for (auto r = row; r < _board.size(); r++) {
            if (0 <= col + abs(r - row) && col + abs(r - row) <= _board.size()){
                if (_board[r][col + abs(r - row)] > 0 && _board[r][col + abs(r - row)] <= 6) {break;}
                moves.push_back(Move(location, {r, col + abs(r - row)}));
                if (_board[r][col + abs(r - row)] > 6) {break;}
            }
        }
    }

    return moves;
}


vector<Move> Board::_GetRookMoves(vector<int> location, bool color) {
    vector<Move> moves;
    int row = location[0];
    int col = location[1];

    // White rook
    if (color) {
        // Top
        for (auto r = row; r > 0; r--) {
            if (_board[r][col] > 6) {break;}
            moves.push_back(Move(location, {r, col}));
            if (_board[r][col] > 0 && _board[r][col] <= 6) {break;}
        }

        // Bottom
        for (auto r = row; r < _board.size(); r++) {
            if (_board[r][col] > 6) {break;}
            moves.push_back(Move(location, {r, col}));
            if (_board[r][col] > 0 && _board[r][col] <= 6) {break;}
        }

        // Left
        for (auto c = col; c > 0; c--) {
            if (_board[row][c] > 6) {break;}
            moves.push_back(Move(location, {row, c}));
            if (_board[row][c] > 0 && _board[row][c] <= 6) {break;}
        }

        // Right
        for (auto c = col; c < _board.size(); c++) {
            if (_board[row][c] > 6) {break;}
            moves.push_back(Move(location, {row, c}));
            if (_board[row][c] > 0 && _board[row][c] <= 6) {break;}
        }
    }

    // Black rook
    else {
        // Top
        for (auto r = row; r > 0; r--) {
            if (_board[r][col] > 0 && _board[r][col] <= 6) {break;}
            moves.push_back(Move(location, {r, col}));
            if (_board[r][col] > 6) {break;}
        }

        // Bottom
        for (auto r = row; r < _board.size(); r++) {
            if (_board[r][col] > 0 && _board[r][col] <= 6) {break;}
            moves.push_back(Move(location, {r, col}));
            if (_board[r][col] > 6) {break;}
        }

        // Left
        for (auto c = col; c > 0; c--) {
            if (_board[row][c] > 0 && _board[row][c] <= 6) {break;}
            moves.push_back(Move(location, {row, c}));
            if (_board[row][c] > 6) {break;}
        }

        // Right
        for (auto c = col; c < _board.size(); c++) {
            if (_board[row][c] > 0 && _board[row][c] <= 6) {break;}
            moves.push_back(Move(location, {row, c}));
            if (_board[row][c] > 6) {break;}
        }
    }

    return moves;
}


vector<Move> Board::_GetQueenMoves(vector<int> location, bool color) {
    vector<Move> moves;

    vector<Move> rook = _GetRookMoves(location, color);
    vector<Move> bishop = _GetBishopMoves(location, color);

    // Queen is combination of rook and bishop
    for (auto i = 0; i < rook.size(); i++) {
        moves.push_back(rook[i]);
    }

    for (auto i = 0; i < bishop.size(); i++) {
        moves.push_back(bishop[i]);
    }

    return moves;
}


vector<Move> Board::_GetKingMoves(vector<int> pieceLoc, bool color) {
    // todo castling
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
        if (locations[i][1] >= 0 && locations[i][0] >= 0 && locations[i][1] <= 7 && locations[i][0] <= 7) {
            currSquare = _board[currLoc[0]][currLoc[1]];
            if (color && (currSquare==0 || (currSquare >= 1 && currSquare <= 6))) {
                moves.push_back(Move(pieceLoc, currLoc));
            } else if (color==false && (currSquare==0 || currSquare >= 7 && currSquare <= 12)) {
                moves.push_back(Move(pieceLoc, currLoc));
            }
        }
    }

    return moves;
}
