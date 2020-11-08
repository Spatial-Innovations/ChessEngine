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
    moves = len(position.move_stack)
    evaluation = 0

    mat = Material(position)
    matWeight = 2 * min(max(0.04*moves + 0.4, 0.8), 1.4)      #* Increases move >= 30, bound = (1.4, 2.8), weight = 2
    center = CenterControl(position)
    centerWeight = 0.5 * min(max(-4*moves/75 + 13/6, 0.3), 1.4)    #* Decreases from move 20 to 35, bound = (0.3, 1.1), weight = 0.5
    dev = Development(position)

    evaluation += mat * matWeight
    evaluation += center * centerWeight
    evaluation += 0.7 * dev

    return int(evaluation * 30)


def Material(position: Board):
    pieces = position.fen().split(" ")[0]
    points = (pieces.count("P") - pieces.count("p"))
    points += 3 * (pieces.count("N") + pieces.count("B") - pieces.count("n") - pieces.count("b"))
    points += 5 * (pieces.count("R") - pieces.count("r"))
    points += 9 * (pieces.count("Q") - pieces.count("q"))

    return points

def Development(position: Board):
    points = 0
    newFen = position.fen().split(" ")[0].replace("/", "").replace("1", " ").replace("2", " "*2).replace("3", " "*3).replace("4", " "*4).replace("5", " "*5).replace("6", " "*6).replace("7", " "*7).replace("8", " "*8)

    for ind, piece in enumerate(newFen):
        if piece.isalpha():
            if piece.isupper():
                attackingSquares = _GetAttackingSquares(newFen, ind, "WHITE")
                points += attackingSquares
            if piece.islower():
                attackingSquares = _GetAttackingSquares(newFen, ind, "BLACK")
                points -= attackingSquares

    return points


def _GetAttackingSquares(position, ind, color):
    color = getattr(chess, color)
    row = ind//8
    col = ind%8
    col = chr(col + 65)
    pos = str(col) + str(row)

    attackers = len(position.attacks(getattr(chess, pos)))
    return attackers


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

    return inner + outer/4
