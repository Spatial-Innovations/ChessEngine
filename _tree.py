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

import threading


class Tree:
    def __init__(self):
        self.processing = False
        self.nodes = 0

    def Go(self, **kwargs):
        threading.Thread(target=self.Stopper, args=()).start()
        self.board = kwargs["board"]
        if "depth" in kwargs:
            pass
        else:
            pass

    def Stopper(self):
        while True:
            if input().strip() == "stop":
                self.processing = False
                return