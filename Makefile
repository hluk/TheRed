# install prefix
prefix = /usr/local
prefix_bin = $(prefix)/bin
prefix_images = "$(prefix)/share/icons/hicolor/scalable/apps"
# required packages for pkg-config
PKGS = gtk+-3.0
# binary filename
EXE = thered
ICON = images/thered.svg
# tools
PKGCONFIG = pkg-config
# default flags
CFLAGS = -O2 -std=c99 -Wextra -Wall -pedantic
FLAGS = $(shell $(PKGCONFIG) --cflags $(PKGS))
LIBS = $(shell $(PKGCONFIG) --libs $(PKGS)) -lX11 -lXxf86vm
DEFINES += -DINSTALL_PREFIX="$(prefix)"
DEFINES += -DINSTALL_PREFIX_IMAGES="$(prefix_images)"

all: $(EXE)

$(EXE): main.c
	$(CC) $< -o $@ $(CFLAGS) $(LDFLAGS) $(FLAGS) $(LIBS) $(DEFINES)

clean:
	$(RM) $(EXE)

install:
	mkdir -p "$(prefix_bin)" "$(prefix_images)"
	sudo install "$(EXE)" "$(prefix_bin)"
	sudo install "$(ICON)" "$(prefix_images)"

