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

# attacking, positional
PERSONALITY = "normal"

if PERSONALITY == "normal":
    WEIGHTS = {"mat": 3, "center": 0.75, "pawn": 0.8, "pieceMap": 0.6}
elif PERSONALITY == "attacking":
    WEIGHTS = {"mat": 3, "center": 0.65, "pawn": 0.77, "pieceMap": 0.63}
elif PERSONALITY == "positional":
    WEIGHTS = {"mat": 3.2, "center": 0.6, "pawn": 0.82, "pieceMap": 0.57}


def Eval(position: Board):
    if position.is_game_over():
        result = position.result()
        if result == "1-0":
            return float("inf")
        elif result == "0-1":
            return float("-inf")
        elif result == "1/2-1/2":
            return 0

    moveNum = len(position.move_stack)

    if PERSONALITY == "dynamic":
        mat = Material(position) * max(min(0.03*moveNum + 1.7, 3), 2)
        center = CenterControl(position) * max(min(-0.015*moveNum + 0.9, 0.7), 0.3)
        pawn = PawnStruct(position) * max(min(-0.002*moveNum + 0.1, 0.1), 0.02)
        pieceMap = Map(position, moveNum) * max(min(-0.002*moveNum + 0.1, 0.1), 0.02)
    else:
        mat = Material(position) * WEIGHTS["mat"]
        center = CenterControl(position) * WEIGHTS["center"]
        pawn = PawnStruct(position) * WEIGHTS["pawn"]
        pieceMap = 0 if moveNum > 25 else Map(position, moveNum) * WEIGHTS["pieceMap"]

    evaluation = mat + center + pawn + pieceMap
    return int(evaluation * 20)


def Map(position, moveNum):
    points = 0
    fen = position.fen().split(" ")[0].replace("/", "").replace("1", " ").replace("2", " "*2).replace("3", " "*3).replace("4", " "*4).replace("5", " "*5).replace("6", " "*6).replace("7", " "*7).replace("8", " "*8)

    if moveNum <= 10:
        for map, pieceType in zip((MAP_BEG_P, MAP_BEG_N, MAP_BEG_B, MAP_BEG_R, MAP_BEG_Q, MAP_BEG_K), ("p", "n", "b", "r", "q", "k")):
            for i, piece in enumerate(fen):
                row, col = i//8, i%8
                if piece.lower() == pieceType:
                    if piece.isupper():
                        points += map[row][col]
                    elif piece.islower():
                        points -= map[7 - row][col]

    elif 10 < moveNum <= 25:
        for map, pieceType in zip((MAP_MID_P, MAP_MID_N, MAP_MID_B, MAP_MID_R, MAP_MID_Q, MAP_MID_K), ("p", "n", "b", "r", "q", "k")):
            for i, piece in enumerate(fen):
                row, col = i//8, i%8
                if piece.lower() == pieceType:
                    if piece.isupper():
                        points += map[row][col]
                    elif piece.islower():
                        points -= map[7-row][col]

    else:
        points = 0

    # Value around 1 pawn
    return points / 100


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

    # Outer center, weighted 0.2
    outer = 0
    squares = ("C3", "D3", "E3", "F3", "F4", "F5", "F6", "E6", "D6", "C6", "C5", "C4")
    for sq in squares:
        outer += len(position.attackers(chess.WHITE, getattr(chess, sq)))
        outer -= len(position.attackers(chess.BLACK, getattr(chess, sq)))

    # Value around 1.5 pawns
    return (inner + outer/5) / 30


def PawnStruct(position: Board):
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
    score = 0.5*((4-whiteIslands)-(4-blackIslands)) + (whitePassed - blackPassed) - 0.1*(whiteStackScore-blackStackScore) + 0.5*(whiteAvgRank-blackAvgRank)
    # Value around 0.8 pawns
    return score / 20