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
#include <string>
#include "board.hpp"
using namespace std;


int main() {
    string msg;
    vector<Move> test;
    Board board;

    while (true) {
        cin >> msg;

        if (msg == "quit") {return 0;}
        else if (msg == "isready") {cout << "readyok" << endl;}
        else if (msg == "uci") {cout << "uciok" << endl;}
        else if (msg == "d") {board.Print();}
        else if (msg == "l") {
            vector<Move> moves = board.GetLegalMoves();
            for (auto i = 0; i < moves.size(); i++) {
                cout << moves[i].GetUci() << endl;
            }
        }
    }
}
