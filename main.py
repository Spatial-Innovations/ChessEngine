#  ##### BEGIN GPL LICENSE BLOCK #####
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <https://www.gnu.org/licenses/>.
#
# ##### END GPL LICENSE BLOCK #####

import sys
import threading
from chess import Board
from _tree import Tree


def Main():
    board = Board()
    tree = Tree()

    while True:
        msg = input().strip()

        if msg == "quit":
            sys.exit()
        elif msg == "isready":
            print("readyok")
        elif msg == "uci":
            print("uciok")
        elif msg == "d":
            print(board)

        elif msg == "ucinewgame":
            board = Board()
        elif msg.startswith("position"):
            msg = msg.replace("position", "").strip()
            if msg.startswith("fen"):
                fen = msg.replace("fen", "").strip()
                board = Board(fen)
            elif msg.startswith("startpos"):
                msg = msg.replace("startpos", "").strip()
                board = Board()
                if msg.startswith("moves"):
                    moves = msg.replace("moves", "").strip().split(" ")
                    for m in moves:
                        board.push_uci(m)

        elif msg.startswith("go"):
            tree.Go(board=board)


Main()