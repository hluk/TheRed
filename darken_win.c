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

#include <windows.h>
#include <wingdi.h>

int darken(int r, int g, int b)
{
    int retval = -1;
    WORD *ramp, *red, *green, *blue;

    HDC hDC = GetDC(NULL);
    if (hDC == NULL)
        return -1;

    ramp = malloc(3 * 256 * sizeof(WORD));
    if (ramp == NULL) {
        perror("malloc");
        ReleaseDC(NULL, hDC);
        return -1;
    }

    if ( GetDeviceGammaRamp(hDC, ramp) ) {
        red   = &ramp[0];
        green = &ramp[256];
        blue  = &ramp[512];

        retval = change_colors(red, green, blue, 256, r, g, b);

        if ( !SetDeviceGammaRamp(hDC, ramp) )
            retval = -1;
    }

    free(ramp);

    ReleaseDC(NULL, hDC);

    return retval;
}

