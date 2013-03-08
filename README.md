         _____ _         _____       _
        |_   _| |_ ___  | __  |___ _| |
          | | |   | -_| |    -| -_| . |
          |_| |_|_|___| |__|__|___|___|

Change intensity of colors from tray.

This is application similar to [f.lux](http://stereopsis.com/flux/) and
[Redshift](http://jonls.dk/redshift/) except that how and when colors are
darkened is completely up to user.

Color intensity can be changed by left and right button or mouse wheel on tray
icon.

Amount to decrement for each color component (red, green and blue) can be
modified by user.

Command Line
------------

    Darken the screen from tray.

    USAGE: ./thered [OPTIONS]
    OPTIONS:
      -h, --help         Print this help.
      -v, --version      Print version.
      -r, --red   VALUE  Amount of red to change   (default is 0).
      -g, --green VALUE  Amount of green to change (default is 16).
      -b, --blue  VALUE  Amount of blue to change  (default is 24).

    Red, green and blue values must be in range from 0 to 255.

The Red (c) 2013 Lukas Holecek (hluk@email.cz)

