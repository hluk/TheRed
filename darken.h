/*
    Copyright (c) 2013, Lukas Holecek <hluk@email.cz>

    This file is part of TheRed.

    TheRed is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TheRed is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TheRed.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * Change default screen colors by adding an amount to each color component.
 * @return -1 on error, otherwise value of change_colors()
 */
int darken(int r, int g, int b);

/**
 * Modify colors of given size.
 * @retval 0 if colors were changed
 * @retval 1 if colors won't be changed further
 */
int change_colors(unsigned short *red, unsigned short *green, unsigned short *blue,
                  int sz, int r, int g, int b);

