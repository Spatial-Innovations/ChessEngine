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


def Eval(position: Board):
    if position.is_game_over():
        result = position.result()
        if result == "1-0":
            return float("inf")
        elif result == "0-1":
            return float("-inf")
        elif result == "1/2-1/2":
            return 0
            
    # todo dynamic weights
    mat = 2 * Material(position)
    center = 0.7 * CenterControl(position)

    evaluation = mat + center

    return int(evaluation * 30)


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

    return (inner + outer/4) / 35


#def PawnStruct(position: Board):
#    pawns = []
#    for piece in position.piece_at()


"""
def EarlyDev(position: Board):
    points = 0
    newFen = position.fen().split(" ")[0].replace("/", "").replace("1", " ").replace("2", " "*2).replace("3", " "*3).replace("4", " "*4).replace("5", " "*5).replace("6", " "*6).replace("7", " "*7).replace("8", " "*8)

    for ind, piece in enumerate(newFen):
        if piece.isalpha() and piece.lower() in ("q", "n", "b"):
            attackingSquares = _GetAttackingSquares(position, ind)
            if piece.isupper():
                points += attackingSquares
            if piece.islower():
                points -= attackingSquares

    return points / 50


def LateDev(position: Board):
    points = 0
    newFen = position.fen().split(" ")[0].replace("/", "").replace("1", " ").replace("2", " "*2).replace("3", " "*3).replace("4", " "*4).replace("5", " "*5).replace("6", " "*6).replace("7", " "*7).replace("8", " "*8)

    for ind, piece in enumerate(newFen):
        if piece.isalpha() and piece.lower() in ("q", "k", "r"):
            attackingSquares = _GetAttackingSquares(position, ind)
            if piece.isupper():
                points += attackingSquares
            if piece.islower():
                points -= attackingSquares

    return points


def _GetAttackingSquares(position, ind):
    row = ind // 8 + 1
    col = ind % 8
    col = chr(col + 65)
    pos = str(col) + str(row)

    attackers = len(position.attacks(getattr(chess, pos)))
    return attackers
"""