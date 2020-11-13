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

import chess
from chess import Board
from _maps import *
from itertools import groupby

MODE = "SLOW"


def Eval(position: Board):
    if position.is_game_over():
        result = position.result()
        if result == "1-0":
            return float("inf")
        elif result == "0-1":
            return float("-inf")
        elif result == "1/2-1/2":
            return 0

    material = Material(position)
    evaluation = material
    
    if MODE == "SLOW":
        pawns = Pawns(position)
        evaluation += pawns
    else:
        center = CenterControl(position)
        evaluation += center
        evaluation *= 20

    return int(evaluation)


def Material(position: Board):
    pieces = position.fen().split(" ")[0]
    points = (pieces.count("P") - pieces.count("p"))
    points += 3 * (pieces.count("N") + pieces.count("B") - pieces.count("n") - pieces.count("b"))
    points += 5 * (pieces.count("R") - pieces.count("r"))
    points += 9 * (pieces.count("Q") - pieces.count("q"))

    return points


def CenterControl(position: Board):
    # Inner center
    inner = 0
    squares = ("D4", "D5", "E4", "E5")
    for sq in squares:
        inner += len(position.attackers(chess.WHITE, getattr(chess, sq)))
        inner -= len(position.attackers(chess.BLACK, getattr(chess, sq)))
    inner /= len(squares)

    # Outer center, weighted 0.2
    outer = 0
    squares = ("C3", "D3", "E3", "F3", "F4", "F5", "F6", "E6", "D6", "C6", "C5", "C4")
    for sq in squares:
        outer += len(position.attackers(chess.WHITE, getattr(chess, sq)))
        outer -= len(position.attackers(chess.BLACK, getattr(chess, sq)))
    outer /= len(squares)

    return (inner + outer/5) / 25


def Pawns(position: Board):
    pawnsW = [(p//8, p%8) for p in position.pieces(chess.PAWN, True)]
    pawnsB = [(p//8, p%8) for p in position.pieces(chess.PAWN, False)]

    # Pawn islands
    whiteCols = [False for _ in range(8)]
    for pawn in pawnsW:
        whiteCols[pawn[1]] = True
    whiteIslands = [i[0] for i in groupby(whiteCols)].count(True)

    blackCols = [False for _ in range(8)]
    for pawn in pawnsB:
        blackCols[pawn[1]] = True
    blackIslands = [i[0] for i in groupby(blackCols)].count(True)

    # Passed pawns
    minRow = float("inf")
    for pawn in pawnsB:
        if (row := pawn[0]) < minRow:
            minRow = row
    whitePassed = 0
    for pawn in pawnsW:
        if pawn[0] <= minRow:
            whitePassed += 1

    maxRow = float("-inf")
    for pawn in pawnsW:
        if (row := pawn[0]) > maxRow:
            maxRow = row
    blackPassed = 0
    for pawn in pawnsB:
        if pawn[0] >= maxRow:
            blackPassed += 1

    # Doubled / Tripled
    whiteStackCount = [0, 0, 0, 0, 0, 0, 0]
    whiteColCount = [0, 0, 0, 0, 0, 0, 0, 0]
    for pawn in pawnsW:
        whiteColCount[pawn[1]] += 1
    for i in range(len(whiteStackCount)):
        whiteStackCount[i] = whiteColCount.count(i+1)

    blackStackCount = [0, 0, 0, 0, 0, 0, 0]
    blackColCount = [0, 0, 0, 0, 0, 0, 0, 0]
    for pawn in pawnsB:
        blackColCount[pawn[1]] += 1
    for i in range(len(blackStackCount)):
        blackStackCount[i] = blackColCount.count(i+1)

    whiteStackScore = 0
    for i in range(len(whiteStackCount)):
        whiteStackScore += i * whiteStackCount[i]
    blackStackScore = 0
    for i in range(len(blackStackCount)):
        blackStackScore += i * blackStackCount[i]

    # Pawn advancement
    if len(pawnsW) == 0:
        whiteAvgRank = 0
    else:
        whiteAvgRank = 0
        for pawn in pawnsW:
            whiteAvgRank += pawn[0]
        whiteAvgRank /= len(pawnsW)

    if len(pawnsB) == 0:
        blackAvgRank = 0
    else:
        blackAvgRank = 0
        for pawn in pawnsW:
            blackAvgRank += 8 - pawn[0]
        blackAvgRank /= len(pawnsB)

    # Final
    score = 0.1*((4-whiteIslands)-(4-blackIslands)) + 0.3*(whitePassed - blackPassed) - 0.1*(whiteStackScore-blackStackScore) + 0.2*(whiteAvgRank-blackAvgRank)
    return score / 2