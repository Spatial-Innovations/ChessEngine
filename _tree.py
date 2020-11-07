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
from copy import deepcopy
from chess import Board
from _eval import QuickEval


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
        pos = kwargs["position"]
        root = Node(pos, 0, self)

        self.turn = pos.turn
        self.processing = True
        self.nodes = 0
        self.timeStart = time.time()
        threading.Thread(target=self.PrintInfo, args=()).start()

        for depth in range(kwargs["depth"]):
            self.depth = depth
            root.GenBranches(depth)

        self.processing = False
        print(self.GetInfoStr())
        print("bestmove")

    def PrintInfo(self):
        #* Prints info regularly.
        while self.processing:
            print(self.GetInfoStr())
            time.sleep(self.printPauseTime)

    def GetInfoStr(self):
        #* Returns info string
        elapseTime = time.time() - self.timeStart
        return self.infoStr.format(depth=self.depth+1, cp=self.score, nodes=self.nodes, nps=int(self.nodes/(elapseTime+1)), time=int(elapseTime*1000), moves=self.currMove)


class Node:
    def __init__(self, pos: Board, depth, tree: Tree):
        self.pos = pos
        self.depth = depth
        self.tree = tree

        self.branches = []

    def Minimax(self, targetDepth, maxPlayer, alpha, beta):
        if self.depth == targetDepth:
            return (QuickEval(self.pos), self.pos.peek())

        newDepth = self.depth + 1
        bestMove = None
        if maxPlayer:
            maxEval = float("-inf")
            for move in self.pos.generate_legal_moves():
                board = deepcopy(self.pos)
                board.push(move)
                node = Node(board, newDepth, self.tree)
                
                evaluation = node.Minimax(targetDepth, False, alpha, beta)[0]
                maxEval = max(maxEval, evaluation)
                alpha = max(alpha, evaluation)
                if maxEval == evaluation:
                    bestMove = move
                if beta <= alpha:
                    break
            
            return (maxEval, bestMove)

        else:
            minEval = float("inf")
            for move in self.pos.generate_legal_moves():
                board = deepcopy(self.pos)
                board.push(move)
                node = Node(board, newDepth, self.tree)

                evaluation = node.Minimax(targetDepth, True, alpha, beta)[0]
                minEval = min(minEval, evaluation)
                if minEval == evaluation:
                    bestMove = move
                beta = min(beta, evaluation)
                if beta <= alpha:
                    break
            
            return (minEval, bestMove)