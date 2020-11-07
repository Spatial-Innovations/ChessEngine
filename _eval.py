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

def QuickEval(position):
    color = position.turn  # * Gets true for white and false for black
    pieces = position.fen().split(" ")[0]  # * Just gets the first part(piece positions) of the fen

    # * Adds material evaluation
    curr_eval = 0
    curr_eval += (pieces.count("P") - pieces.count("p"))
    curr_eval += 3 * (pieces.count("N") + pieces.count("B") - pieces.count("n") - pieces.count("b"))
    curr_eval += 5 * (pieces.count("R") - pieces.count("r"))
    curr_eval += 9 * (pieces.count("Q") - pieces.count("q"))



'''
# * Checks for mate
result = position.result()  # * if white wins returns '1-0' if black wins returns '0-1' else returns '*'
if result != "*":  # * No one has won
    if bool(result[0]):  # * if the first index of the result is 1
        curr_eval = float("inf")
    else:
        curr_eval = -float("inf")

return curr_eval
'''