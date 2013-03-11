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

#ifdef HAS_SHORTCUTS
#   include "libkeybinder/keybinder.h"
#endif

#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// default option values
#define DEFAULT_RED 0
#define DEFAULT_GREEN 12
#define DEFAULT_BLUE 24

#define MouseButtonLeft 1
#define MouseButtonMiddle 2
#define MouseButtonRight 3

#define UNUSED(x) (void)x

#ifdef HAS_SHORTCUTS
#   define SHORTCUT_SIZE 256
#endif // HAS_SHORTCUTS

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
    short min_x;
#ifdef HAS_SHORTCUTS
    char shortcut_increase[SHORTCUT_SIZE];
    char shortcut_decrease[SHORTCUT_SIZE];
#endif // HAS_SHORTCUTS
} app;

/**
 * Bound number to given range.
 */
int bound(int min, int x, int max)
{
    return ( x < min ) ? min : ( x > max ) ? max : x;
}

void app_init()
{
    app.tray = NULL;
    app.r = DEFAULT_RED;
    app.g = DEFAULT_GREEN;
    app.b = DEFAULT_BLUE;
    app.x = 0;
#ifdef HAS_SHORTCUTS
    app.shortcut_increase[0] = '\0';
    app.shortcut_decrease[0] = '\0';
#endif // HAS_SHORTCUTS
}

void app_update_tray_icon()
{
    char filename[] = str(INSTALL_PREFIX_IMAGES) "/thered_0.svg";
    filename[sizeof(filename) * sizeof(char) - 6] = '0' + bound(0, app.x * 14 / app.min_x, 9);
    gtk_status_icon_set_from_file(app.tray, filename);
}

void app_set_dark(int x)
{
    if (x < app.min_x)
        return;

    int r = bound(0, 255 + x * app.r, 255);
    int g = bound(0, 255 + x * app.g, 255);
    int b = bound(0, 255 + x * app.b, 255);

    if ( darken(r, g, b) == 0 ) {
        app.x = x;
        app_update_tray_icon();
    }
}

/**
 * Reset screen colors to default values.
 */
void reset_colors()
{
    if ( darken(0, 0, 0) == -1 ) {
        g_error("ERROR: Cannot set screen colors!\n");
        exit(-1);
    }
}

void print_build_info()
{
    printf("Build with: GTK+ %d.%d.%d\n", GTK_MAJOR_VERSION, GTK_MINOR_VERSION, GTK_MICRO_VERSION);
}

void print_version()
{
    printf("The Red 1.0 (c) 2013 Lukas Holecek (hluk@email.cz)\n");
}

void print_help(const char *cmd)
{
    printf("Darken the screen from tray.\n");
    printf("\n");
    printf("USAGE: %s [OPTIONS]\n", cmd);
    printf("OPTIONS:\n");
    printf("  -h, --help         Print this help.\n");
    printf("  -v, --version      Print version.\n");
    printf("\n");
    printf("  -r, --red   VALUE  Amount of red to change   (default is " str(DEFAULT_RED)   ").\n");
    printf("  -g, --green VALUE  Amount of green to change (default is " str(DEFAULT_GREEN) ").\n");
    printf("  -b, --blue  VALUE  Amount of blue to change  (default is " str(DEFAULT_BLUE)  ").\n");
    printf("\n");
#ifdef HAS_SHORTCUTS
    printf("  -d, --darken  SHORTCUT  System-wide shortcut to darken screen.\n");
    printf("  -l, --lighten SHORTCUT  System-wide shortcut to lighten screen.\n");
    printf("\n");
    printf("Argument SHORTCUT must have correct format. "
           "It can be for example \"<Ctrl><Alt>D\" or \"<Super>X\".\n");
    printf("\n");
#endif // HAS_SHORTCUTS
    printf("Red, green and blue values must be in range from 0 to 255.\n");
    printf("\n");
    print_version();
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
        app_set_dark(app.x + 1);
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

const char *parse_next_command_line_argument(int argc, char **argv, int *i)
{
    if (*i + 1 == argc) {
        fprintf(stderr, "Missing color value for option \"%s\"!\n", argv[*i]);
        exit(2);
    }

    ++*i;
    return argv[*i];
}

gboolean parse_command_line_colors(int argc, char **argv, int *i)
{
    unsigned short *color = NULL;
    const char *arg = argv[*i];
    if ( is_arg(arg, 'r', "red") )
        color = &app.r;
    else if ( is_arg(arg, 'g', "green") )
        color = &app.g;
    else if ( is_arg(arg, 'b', "blue") )
        color = &app.b;

    if (color == NULL)
        return FALSE;

    arg = parse_next_command_line_argument(argc, argv, i);
    int value = atoi(arg);

    if (value < 0 || value > 255) {
        fprintf(stderr, "Color value must be in range from 0 to 255!\n");
        exit(2);
    }

    *color = value;

    return TRUE;
}

#ifdef HAS_SHORTCUTS
gboolean parse_command_line_shortcuts(int argc, char **argv, int *i)
{
    UNUSED(argc);

    const char *arg = argv[*i];

    char *shortcut = NULL;
    if ( is_arg(arg, 'd', "darken-shortcut") )
        shortcut = app.shortcut_decrease;
    else if ( is_arg(arg, 'l', "lighten-shortcut") )
        shortcut = app.shortcut_increase;

    if (shortcut == NULL)
        return FALSE;

    arg = parse_next_command_line_argument(argc, argv, i);
    strncpy(shortcut, arg, SHORTCUT_SIZE);

    return TRUE;
}
#endif // HAS_SHORTCUTS

void parse_command_line(int argc, char **argv)
{
    int i;
    for (i = 1; i < argc; ++i) {
        if ( is_arg(argv[i], 'h', "help") ) {
            print_help(argv[0]);
            exit(0);
        } else if ( is_arg(argv[i], 'v', "version") ) {
            print_version();
            print_build_info();
            exit(0);
        } else {
            if ( !parse_command_line_colors(argc, argv, &i)
#ifdef HAS_SHORTCUTS
                 && !parse_command_line_shortcuts(argc, argv, &i)
#endif // HAS_SHORTCUTS
                 )
            {
                fprintf(stderr, "Unknown option \"%s\"!\n", argv[i]);
                exit(2);
            }
        }
    }

    app.min_x = - (3 * 255 - 128) / (app.r + app.g + app.b);
}

#ifdef HAS_SHORTCUTS
void on_shortcut_increase_pressed(const char *keystring, void *data)
{
    UNUSED(data);
    UNUSED(keystring);

    app_set_dark(app.x + 1);
}

void on_shortcut_decrease_pressed(const char *keystring, void *data)
{
    UNUSED(data);
    UNUSED(keystring);

    app_set_dark(app.x - 1);
}

void init_shortcut(const char *shortcut, KeybinderHandler handler)
{
    if ( !keybinder_bind(shortcut, handler, NULL) )
        fprintf(stderr, "Cannot register shortcut \"%s\"!\n", shortcut);
}

void init_shortcuts()
{
    keybinder_init();
    if (app.shortcut_increase[0] != '\0')
        init_shortcut(app.shortcut_increase, on_shortcut_increase_pressed);
    if (app.shortcut_decrease[0] != '\0')
        init_shortcut(app.shortcut_decrease, on_shortcut_decrease_pressed);
}
#endif // HAS_SHORTCUTS

int main(int argc, char **argv)
{
    app_init();

    parse_command_line(argc, argv);

    gtk_init(&argc, &argv);

    create_tray_icon();

#ifdef HAS_SHORTCUTS
    init_shortcuts();
#endif // HAS_SHORTCUTS

    gtk_main();

    return 0;
}
