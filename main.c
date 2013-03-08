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

#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>

// default option values
#define DEFAULT_RED 0
#define DEFAULT_GREEN 16
#define DEFAULT_BLUE 24

#define MouseButtonLeft 1
#define MouseButtonMiddle 2
#define MouseButtonRight 3

#define UNUSED(x) (void)x

// stringify
#define strx(x) #x
#define str(x) strx(x)

/**
 * Options and widgets for the application.
 */
struct {
    GtkStatusIcon *tray;
    unsigned short r;
    unsigned short g;
    unsigned short b;
    short x;
} app;

/**
 * Bound number to given range.
 */
int bound(int min, int x, int max)
{
    return ( x < min ) ? min : ( x > max ) ? max : x;
}

/**
 * Change default screen colors by adding an amount to each color component.
 */
gboolean change_colors(int r, int g, int b)
{
    int sz, i;
    unsigned short *red, *green, *blue;

    Display *dpy = XOpenDisplay(NULL);
    XF86VidModeGetGammaRampSize(dpy, 0, &sz);

    red = malloc(sz * sizeof(unsigned short));
    green = malloc(sz * sizeof(unsigned short));
    blue = malloc(sz * sizeof(unsigned short));

    XF86VidModeGetGammaRamp(dpy, 0, sz, red, green, blue);

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
            return FALSE;
        }

        int rx = 0;
        int gx = 0;
        int bx = 0;
        for (i = 0; i < sz; i++) {
            red[i]   = bound(0, rx, i*sz);
            green[i] = bound(0, gx, i*sz);
            blue[i]  = bound(0, bx, i*sz);
            rx = bound(0, rx + rxx, max);
            gx = bound(0, gx + gxx, max);
            bx = bound(0, bx + bxx, max);
        }
    }

    XF86VidModeSetGammaRamp(dpy, 0, sz, red, green, blue);

    free(red);
    free(green);
    free(blue);
    XCloseDisplay(dpy);

    return TRUE;
}

void app_init()
{
    app.tray = NULL;
    app.r = DEFAULT_RED;
    app.g = DEFAULT_GREEN;
    app.b = DEFAULT_BLUE;
    app.x = 0;
}

void app_update_tray_icon()
{
    char filename[] = "images/thered_0.svg";
    filename[sizeof(filename) * sizeof(char) - 6] = '0' + bound(0, -app.x / 2, 9);
    gtk_status_icon_set_from_file(app.tray, filename);
}

void app_set_dark(int x)
{
    int r = bound(0, 255 + x * app.r, 255);
    int g = bound(0, 255 + x * app.g, 255);
    int b = bound(0, 255 + x * app.b, 255);
    if ( r + g + b > 128 ) {
        if ( change_colors(r, g, b) ) {
            app.x = x;
            app_update_tray_icon();
        }
    }
}

/**
 * Reset screen colors to default values.
 */
void reset_colors()
{
    change_colors(0, 0, 0);
}

void print_simple_version()
{
    printf("The Red 1.0 (c) 2013 Lukas Holecek (hluk@email.cz)\n");
    printf("Build with: GTK+ %d.%d.%d\n", GTK_MAJOR_VERSION, GTK_MINOR_VERSION, GTK_MICRO_VERSION);
}

void print_version()
{
    printf("         _____ _         _____       _\n");
    printf("        |_   _| |_ ___  | __  |___ _| |\n");
    printf("          | | |   | -_| |    -| -_| . |\n");
    printf("          |_| |_|_|___| |__|__|___|___|\n");
    printf("\n");
    print_simple_version();
}

void print_help(const char *cmd)
{
    printf("Darken the screen from tray.\n");
    printf("\n");
    printf("USAGE: %s [OPTIONS]\n", cmd);
    printf("OPTIONS:\n");
    printf("  -h, --help         Print this help.\n");
    printf("  -v, --version      Print version.\n");
    printf("  -r, --red   VALUE  Amount of red to change   (default is " str(DEFAULT_RED)   ").\n");
    printf("  -g, --green VALUE  Amount of green to change (default is " str(DEFAULT_GREEN) ").\n");
    printf("  -b, --blue  VALUE  Amount of blue to change  (default is " str(DEFAULT_BLUE)  ").\n");
    printf("\n");
    printf("Red, green and blue values must be in range from 0 to 255.\n");
    printf("\n");
    print_simple_version();
}

gboolean is_arg(const char *arg, char shortOption, const char *longOption)
{
    if (arg[0] != '-')
        return FALSE;

    if (arg[1] == shortOption && arg[2] == '\0')
        return TRUE;

    return arg[1] == '-' && strcmp(longOption, arg+2) == 0;
}

gboolean test_error(GError **error)
{
    if (*error == NULL)
        return TRUE;

    g_error("ERROR: %s\n", (*error)->message);
    g_error_free(*error);
    *error = NULL;

    return FALSE;
}

gboolean on_tray_button_press(GtkStatusIcon *status_icon,
                              GdkEvent      *event,
                              gpointer       user_data)
{
    UNUSED(status_icon);
    UNUSED(user_data);

    guint button = event->button.button;

    if (button == MouseButtonMiddle) {
        reset_colors();
        gtk_main_quit();
    } else if (button == MouseButtonLeft ) {
        app_set_dark(app.x + 1);
    } else if (button == MouseButtonRight ) {
        app_set_dark(app.x - 1);
    } else {
        return FALSE;
    }

    return TRUE;
}

gboolean on_tray_scroll(GtkStatusIcon *status_icon,
                        GdkEvent      *event,
                        gpointer       user_data)
{
    UNUSED(status_icon);
    UNUSED(user_data);

    GdkScrollDirection scroll = event->scroll.direction;

    if (scroll == GDK_SCROLL_UP ) {
        app_set_dark(app.x - 1);
    } else if (scroll == GDK_SCROLL_DOWN) {
        app_set_dark(app.x - 1);
    } else {
        return FALSE;
    }

    return TRUE;
}

void create_tray_icon()
{
    app.tray = gtk_status_icon_new();
    app_set_dark(0);
    app_update_tray_icon();

    g_signal_connect( app.tray, "button-press-event",
                      G_CALLBACK(on_tray_button_press), NULL );
    g_signal_connect( app.tray, "scroll-event",
                      G_CALLBACK(on_tray_scroll), NULL );
}

void parse_command_line(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i) {
        if ( is_arg(argv[i], 'h', "help") ) {
            print_help(argv[0]);
            exit(0);
        } else if ( is_arg(argv[i], 'v', "version") ) {
            print_version();
            exit(0);
        } else {
            unsigned short *color = NULL;
            if ( is_arg(argv[i], 'r', "red") )
                color = &app.r;
            else if ( is_arg(argv[i], 'g', "green") )
                color = &app.g;
            else if ( is_arg(argv[i], 'b', "blue") )
                color = &app.b;

            if (color == NULL) {
                fprintf(stderr, "Unknown option \"%s\"!\n", argv[i]);
                exit(2);
            }

            if (i + 1 == argc) {
                fprintf(stderr, "Missing color value for option \"%s\"!\n", argv[i]);
                exit(2);
            }

            int value = atoi(argv[++i]);

            if (value < 0 || value > 255) {
                fprintf(stderr, "Color value must be in range from 0 to 255!\n");
                exit(2);
            }

            *color = value;
        }
    }
}

int main(int argc, char **argv)
{
    app_init();

    parse_command_line(argc, argv);

    gtk_init(&argc, &argv);

    create_tray_icon();

    gtk_main();

    return 0;
}
