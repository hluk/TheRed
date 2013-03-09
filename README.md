The Red
=======

Change intensity of screen colors from tray.

This is application similar to [f.lux](http://stereopsis.com/flux/) and
[Redshift](http://jonls.dk/redshift/) except that how and when colors are
darkened is completely up to user.

Color intensity can be changed by clicking with left and right button or
scrolling with mouse wheel on tray icon.

Amount to decrement for each color component (red, green and blue) can be
modified by user (by passing arguments to the application).

Command Line
------------

    Darken the screen from tray.

    USAGE: ./thered [OPTIONS]
    OPTIONS:
      -h, --help         Print this help.
      -v, --version      Print version.

      -r, --red   VALUE  Amount of red to change   (default is 0).
      -g, --green VALUE  Amount of green to change (default is 12).
      -b, --blue  VALUE  Amount of blue to change  (default is 24).

      -d, --darken  SHORTCUT  System-wide shortcut to darken screen.
      -l, --lighten SHORTCUT  System-wide shortcut to lighten screen.

    Argument SHORTCUT must have correct format. It can be for example "<Ctrl><Alt>D" or "<Super>X".

    Red, green and blue values must be in range from 0 to 255.

The Red (c) 2013 Lukas Holecek (hluk@email.cz)

