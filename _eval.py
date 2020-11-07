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
    # Material Count
    pieces = position.fen().split(" ")[0]
    points = (pieces.count("P") - pieces.count("p"))
    points += 3* (pieces.count("N") + pieces.count("B") - pieces.count("n") - pieces.count("b"))
    points += 5* (pieces.count("R") - pieces.count("r"))
    points += 9* (pieces.count("Q") - pieces.count("q"))

    # Center Control
    # Var names like this:
    # Ind 1: color (b or w)
    # Ind 2: top or bottom (t or b) - for center square
    # Ind 3: left or right (l or r) - for center square
    btl = len(position.attackers(chess.BLACK, chess.D5))
    btr = len(position.attackers(chess.BLACK, chess.E5))
    bbl = len(position.attackers(chess.BLACK, chess.D4))
    bbr = len(position.attackers(chess.BLACK, chess.E4))
    wtl = len(position.attackers(chess.WHITE, chess.D5))
    wtr = len(position.attackers(chess.WHITE, chess.E5))
    wbl = len(position.attackers(chess.WHITE, chess.D4))
    wbr = len(position.attackers(chess.WHITE, chess.E4))

    control = (wtl + wtr + wbl + wbr) - (btl + btr + bbl + bbr)
    points += control/2

    return points