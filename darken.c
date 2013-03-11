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

#include "darken.h"

int bound(int min, int x, int max)
{
    return ( x < min ) ? min : ( x > max ) ? max : x;
}

int change_colors(unsigned short *red, unsigned short *green, unsigned short *blue,
                  int sz, int r, int g, int b)
{
    int i, rx, gx, bx;

    if (r == 0 && g == 0 && b == 0) {
        int x = 0;
        for (i = 0; i < sz; i++) {
            red[i] = green[i] = blue[i] = x;
            x += sz;
        }
    } else {
        const int max = sz * sz;

        const int rxx = r * sz / 255;
        const int gxx = g * sz / 255;
        const int bxx = b * sz / 255;

        if ( red[1]   == bound(0, rxx, sz) &&
             green[1] == bound(0, gxx, sz) &&
             blue[1]  == bound(0, bxx, sz) )
        {
            return 1;
        }

        rx = 0;
        gx = 0;
        bx = 0;
        for (i = 0; i < sz; i++) {
            red[i]   = bound(0, rx, i*sz);
            green[i] = bound(0, gx, i*sz);
            blue[i]  = bound(0, bx, i*sz);
            rx = bound(0, rx + rxx, max);
            gx = bound(0, gx + gxx, max);
            bx = bound(0, bx + bxx, max);
        }
    }

    return 0;
}

