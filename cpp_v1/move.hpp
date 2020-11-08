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
#include <string>
using namespace std;

class Move {
    public:
        ~Move();
        Move(vector<int>, vector<int>, string);

        bool IsPromotion(void);
        string GetFirst(void);
        string GetSecond(void);
        string GetPromotion(void);
        string GetUci(void);

    private:
        vector<int> _first;
        vector<int> _second;
        bool _promotion;
        string _promotionPiece;

        string _GetSquare(int, int);
};