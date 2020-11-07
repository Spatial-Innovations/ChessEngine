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
            for depth in range(kwargs["depth"]):
                if not self.processing:
                    break
                self.depth = depth
                self.root.Branch(depth)
        elif "nodes" in kwargs:
            threading.Thread(target=self.TimerNodes, args=(kwargs["nodes"],)).start()
            for depth in range(10000):
                if not self.processing:
                    break
                self.depth = depth
                self.root.Branch(depth)
        else:
            for depth in range(10000):
                if not self.processing:
                    break
                self.depth = depth
                self.root.Branch(depth)

        self.processing = False
        self.PrintStr()
        print(f"bestmove {self.bestMove}")
        del self.root

    def Printer(self):
        expBase = 15000   #* Print every time self.nodes reaches the next multiple of this number.
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
        try:
            self.bestMove = self.root.Minimax(False, float("-inf"), float("inf"))[1]
        except:
            self.bestMove = None
        string = self.infoStr.format(depth=self.depth, cp=0, nodes=self.nodes, nps=int(self.nodes/(timeElapse+1)), time=int(timeElapse*1000), moves=self.bestMove.uci())
        print(string)
    
    def Stopper(self):
        while True:
            if input().strip() == "stop":
                self.processing = False
                return

    def TimerNodes(self, nodes):
        while self.nodes < nodes:
            time.sleep(0.01)
        self.processing = False


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
                return (self.eval, self.board.peek())
            else:
                self.eval = Eval(self.board)
                return (self.eval, self.board.peek())

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
                evaluation = b.Minimax(True, alpha, beta)[0]
                minEval = min(minEval, evaluation)
                beta = min(beta, evaluation)
                if minEval == evaluation:
                    bestMove = b.board.peek()
                if beta <= alpha:
                    break

            return (minEval, bestMove)