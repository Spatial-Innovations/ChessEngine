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
#include "board.hpp"
#include "move.hpp"
using namespace std;


class Node {
    public:
        ~Node();
        Node();
        Node(vector<Move>, int, int&);

        void GenBranches(int);
        void ResetBranches(void);
        void SetNodeVar(int&);
        void SetMoves(vector<Move>);
        void PushMove(Move);

    private:
        int _depth;
        int _nodes;
        Board _board;
        vector<Node> _branches;
};