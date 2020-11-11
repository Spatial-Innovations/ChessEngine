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
PERSONALITY = "positional"

if PERSONALITY == "attacking":
    WEIGHTS = {"mat": 1.9, "center": 0.6, "pawn": 0.1, "pieceMap": 0.07}
elif PERSONALITY == "positional":
    WEIGHTS = {"mat": 2.1, "center": 0.55, "pawn": 0.12, "pieceMap": 0.05}


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

    #todo fix bug in dynamic weights
    #mat = Material(position) * max(min(0.03*moveNum + 1.7, 3), 2)
    #center = CenterControl(position) * max(min(-0.015*moveNum + 0.9, 0.7), 0.3)
    #pawn = PawnStruct(position) * max(min(-0.002*moveNum + 0.1, 0.1), 0.02)
    #pieceMap = Map(position) * max(min(-0.002*moveNum + 0.1, 0.1), 0.02)

    mat = Material(position) * WEIGHTS["mat"]
    center = CenterControl(position) * WEIGHTS["center"]
    pawn = PawnStruct(position) * WEIGHTS["pawn"]
    pieceMap = 0 if moveNum > 20 else Map(position) * WEIGHTS["pieceMap"]

    evaluation = mat + center + pawn + pieceMap
    return int(evaluation * 30)


def Map(position):
    points = 0
    fen = position.fen().split(" ")[0].replace("/", "").replace("1", " ").replace("2", " "*2).replace("3", " "*3).replace("4", " "*4).replace("5", " "*5).replace("6", " "*6).replace("7", " "*7).replace("8", " "*8)

    for map, pieceType in zip((MAP_P, MAP_N, MAP_B, MAP_R, MAP_Q, MAP_K), ("p", "n", "b", "r", "q", "k")):
        for i, piece in enumerate(fen):
            row, col = i//8, i%8
            if piece.lower() == pieceType:
                if piece.isupper():
                    points += map[row][col]
                elif piece.islower():
                    points -= map[row][col]

    return points


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

    # Outer center, weighted 0.25
    outer = 0
    squares = ("C3", "D3", "E3", "F3", "F4", "F5", "F6", "E6", "D6", "C6", "C5", "C4")
    for sq in squares:
        outer += len(position.attackers(chess.WHITE, getattr(chess, sq)))
        outer -= len(position.attackers(chess.BLACK, getattr(chess, sq)))

    return (inner + outer/4) / 20


def PawnStruct(position: Board):
    # todo doubled/triped
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

    # Final
    return ((4-whiteIslands) - (4-blackIslands)) + (whitePassed - blackPassed)