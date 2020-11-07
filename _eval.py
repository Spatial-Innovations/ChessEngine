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
    # color = position.turn  #* Gets true for white and false for black
    pieces = position.fen().split(" ")[0]  #* Just gets the first part(piece positions) of the fen

    #* Adds material evaluation
    currEval = (pieces.count("P") - pieces.count("p"))
    currEval += 3* (pieces.count("N") + pieces.count("B") - pieces.count("n") - pieces.count("b"))
    currEval += 5* (pieces.count("R") - pieces.count("r"))
    currEval += 9* (pieces.count("Q") - pieces.count("q"))
    return currEval



'''
#* Checks for mate
result = position.result()  #* if white wins returns '1-0' if black wins returns '0-1' else returns '*'
if result != "*":  #* No one has won
    if result.startswith("1-"):  #* if white won
        currEval = float("inf")
    else:
        currEval = -float("inf")
'''