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


class Position {
    // empty=0, wpawn=1, wknight=2, wbishop=3, wrook=4, wqueen=5, wking=6, bpawn=7, bknight=8, bbishop=9, brook=10, bqueen=11, bking=12
    public:
        ~Position();
        Position();

        void Reset(void);
        void Print(void);
        void Push(vector<int>, vector<int>);

        int GetNumNodes(void);
        string GetFen(void);
        vector<vector<vector<int>>> GetMoveStack(void);

    private:
        vector<vector<int>> _position;
        bool _turn;
        vector<bool> _castleRights;
        bool _epLegal;
        vector<int> _epSquare;

        vector<vector<vector<int>>> _moveStack;
        vector<Position> _branches;

        int _PieceToInt(string);
        string _IntToPiece(int);
        string _CoordsToUci(vector<int>);
        vector<int> _UciToCoords(string);

        vector<int> _GetKingPos(bool);
};