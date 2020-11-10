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
#include <chrono>
#include "node.hpp"
#include "move.hpp"
#include "tree.hpp"
using namespace std;


Tree::~Tree() {}
Tree::Tree() {}


void Tree::Set(vector<Move> moves) {
    _root.ResetBranches();
    _root.SetNodeVar(_currNodes);
    _root.SetMoves(moves);

    _currDepth = 0;
    _currNodes = 0;
    _processing = false;
    _ready = true;
}


void Tree::PrintInfo(void) {
    string printStr = "info ";
    printStr += "depth " + to_string(_currDepth) + " seldepth " + to_string(_currDepth);
    printStr += " multipv 1 score cp 0 ";
    printStr += "nodes " + to_string(_currNodes) + " nps " + to_string(_currNodes);
    printStr += " tbhits 0 time 0 pv e2e4";
    cout << printStr;
}


void Tree::GoDepth(int finalDepth) {
    if (_ready == false) {return;}
    _ready = false;
    _processing = true;

    for (auto depth = 0; depth < finalDepth; depth++) {
        _currDepth = depth;
        _root.GenBranches(depth);
    }

    _processing = false;
}
