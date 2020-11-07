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
from chess import Board, Move
from _eval import Eval


class Tree:
    infoStr = "info depth {depth} seldepth {depth} multipv 1 score cp {cp} nodes {nodes} nps {nps} tbhits 0 time {time} pv {moves}"

    def __init__(self):
        self.processing = False
        self.nodes = 0
        self.depth = 0

    def Go(self, **kwargs):
        threading.Thread(target=self.Stopper, args=()).start()
        threading.Thread(target=self.Printer, args=()).start()

        self.processing = True
        self.nodes = 0
        self.board = kwargs["board"]
        self.root = Node(self, self.board, 0)
        self.timeStart = time.time()
        if "depth" in kwargs:
            pass
        else:
            for depth in range(10000):
                self.depth = depth
                self.root.Branch(depth)

        self.processing = False
        print("bestmove")
        del self.root

    def Printer(self):
        expBase = 25000   #* Print every time self.nodes reaches the next multiple of this number.
        currExp = 0
        print()
        while True:
            while self.nodes < expBase*currExp:
                if not self.processing:
                    return
                time.sleep(0.01)

            currExp += 1
            self.PrintStr()

    def PrintStr(self):
        timeElapse = time.time() - self.timeStart
        string = self.infoStr.format(depth=self.depth, cp=0, nodes=self.nodes, nps=int(self.nodes/timeElapse), time=int(timeElapse*1000), moves=None)
        print(string)
    
    def Stopper(self):
        while True:
            if input().strip() == "stop":
                self.processing = False
                return


class Node:
    def __init__(self, tree: Tree, board: Board, depth):
        self.tree = tree
        self.board = board
        self.depth = depth
        self.branches = []

        tree.nodes += 1
        self.legalMoves = list(board.generate_legal_moves())

    def Branch(self, targetDepth):
        if targetDepth == self.depth + 1:
            newDepth = self.depth + 1
            for move in self.legalMoves:
                if not self.tree.processing:
                    return
                newBoard = deepcopy(self.board)
                newBoard.push(move)
                newNode = Node(self.tree, newBoard, newDepth)
                self.branches.append(newNode)

        elif targetDepth > self.depth + 1:
            for b in self.branches:
                if not self.tree.processing:
                    return
                b.Branch(targetDepth)

    def Minimax(self, maxPlayer, alpha, beta):
        if len(self.branches) == 0:
            if hasattr(self, "eval"):
                return self.eval
            else:
                self.eval = Eval(self.board)
                return self.eval

        if maxPlayer:
            maxEval = float("-inf")
            bestMove = None
            for b in self.branches:
                evaluation = b.Minimax(False, alpha, beta)[0]
                maxEval = max(maxEval, evaluation)
                alpha = max(alpha, evaluation)
                if maxEval == evaluation:
                    bestMove = b.board.peek()
                if beta <= alpha:
                    break

            return (maxEval, bestMove)

        else:
            minEval = float("inf")
            bestMove = None
            for b in self.branches:
                evaluation = b.Minimax(True, alpha, beta)
                minEval = min(minEval, evaluation)
                beta = min(beta, evaluation)
                if minEval == evaluation:
                    bestMove = b.board.peek()
                if beta <= alpha:
                    break

            return (minEval, bestMove)