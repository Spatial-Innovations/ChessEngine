//  ##### BEGIN GPL LICENSE BLOCK #####
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
//  ##### END GPL LICENSE BLOCK #####

#include <vector>
using namespace std;

class Board {
    /*
    empty=0, p=1, n=2, b=3, r=4, q=5, k=6, P=1, N=2, B=3, R=4, Q=5, K=6
    */
    public:
        ~Board();
        Board();

    private:
        vector<int> _board;
};