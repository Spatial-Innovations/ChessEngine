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

import time
import threading
from chess import Board


class Tree:
    infoStr = "info depth {depth} seldepth {depth} multipv 1 score cp {cp} nodes {nodes} nps {nps} tbhits 0 time {time} pv {moves}"
    printPauseTime = 1

    def __init__(self):
        self.processing = False
        self.nodes = 0
        self.timeStart = 0
        self.depth = 0
        self.score = 0
        self.currMove = 0

    def Go(self, **kwargs):
        self.processing = True
        self.nodes = 0

        pos = kwargs["position"]
        root = Node(pos, 0, self)
        self.timeStart = time.time()
        threading.Thread(target=self.PrintInfo, args=()).start()
        
        for depth in range(3):
            root.GenBranches(depth)
        
        self.processing = False
        print("bestmove")

    def PrintInfo(self):
        while self.processing:
            elapseTime = time.time() - self.timeStart
            print(self.infoStr.format(depth=self.depth, cp=self.score, nodes=self.nodes, nps=int(self.nodes/(elapseTime+1)), time=int(elapseTime), moves=self.currMove))
            time.sleep(self.printPauseTime)


class Node:
    def __init__(self, pos: Board, depth, tree: Tree):
        self.pos = pos
        self.depth = depth
        self.tree = tree
        
        self.branches = []
        self.fen = pos.fen()

    def GenBranches(self, targetDepth):
        if targetDepth > self.depth:
            for b in self.branches:
                b.GenBranches(targetDepth)
        
        else:
            for move in self.pos.generate_legal_moves():
                board = Board(self.fen)
                board.push(move)
                node = Node(board, self.depth+1)
                self.branches.append(node)
                self.tree.nodes += 1