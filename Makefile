# install prefix
prefix = /usr/local
# required packages for pkg-config
PKGS = gtk+-3.0
# binary filename
EXE = thered
# tools
PKGCONFIG = pkg-config
# default flags
CFLAGS = -O2 -std=c99 -Wextra -Wall -pedantic
FLAGS = $(shell $(PKGCONFIG) --cflags $(PKGS))
LIBS = $(shell $(PKGCONFIG) --libs $(PKGS)) -lX11 -lXxf86vm

all: $(EXE)

$(EXE): main.c
	$(CC) $< -o $@ $(CFLAGS) $(LDFLAGS) $(FLAGS) $(LIBS)

clean:
	$(RM) $(EXE)

install:
	sudo cp $(EXE) $(prefix)/bin/

