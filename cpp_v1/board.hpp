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
using namespace std;

typedef pair<string, string> Move;

class Board {
    //* empty=0, p=1, n=2, b=3, r=4, q=5, k=6, P=7, N=8, B=9, R=10, Q=11, K=12
    public:
        // constructors and destructor
        ~Board();
        Board();

        // interface
        void Print(void);
        vector<Move> GetLegalMoves(void);

    private:
        // member variables
        vector<vector<int>> _board;
        string const _printLine = " +---+---+---+---+---+---+---+---+\n";
        string const _printSplit = " | ";

        // member functions
        string _GetSymbol(int num);
        string _GetSquare(vector<int> coords);

        vector<Move> GetPawnMoves(vector<int> location, bool color);
        vector<Move> GetKnightMoves(vector<int> location, bool color);
        vector<Move> GetBishopMoves(vector<int> location, bool color);
        vector<Move> GetRookMoves(vector<int> location, bool color);
        vector<Move> GetQueenMoves(vector<int> location, bool color);
        vector<Move> GetKingMoves(vector<int> location, bool color);
};