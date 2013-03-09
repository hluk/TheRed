# install prefix
prefix = /usr/local
prefix_bin = $(prefix)/bin
prefix_images = "$(prefix)/share/icons/hicolor/scalable/apps"
# binary filename
EXE = thered
ICON = images/thered.svg
SRCS = main.c

# required packages for pkg-config
PKGS =

# GTK version 3.0 or 2.0
WITH_GTK3 = 1
ifeq ($(WITH_GTK3),1)
    PKGS += gtk+-3.0
else
    PKGS += gtk+-2.0
endif

# system-wide shortcuts
WITH_SHORTCUTS = 1
ifeq ($(WITH_SHORTCUTS),1)
    DEFINES += -DHAS_SHORTCUTS
    SRCS += libkeybinder/bind.c libkeybinder/keybinder.h
endif

# tools
PKGCONFIG = pkg-config

# default flags
CFLAGS = -O2 -std=c99 -Wextra -Wall -pedantic
FLAGS = $(shell $(PKGCONFIG) --cflags $(PKGS))
LIBS = $(shell $(PKGCONFIG) --libs $(PKGS)) -lX11 -lXxf86vm
DEFINES += -DINSTALL_PREFIX="$(prefix)"
DEFINES += -DINSTALL_PREFIX_IMAGES="$(prefix_images)"

all: $(EXE)

$(EXE): $(SRCS)
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS) $(FLAGS) $(LIBS) $(DEFINES)

clean:
	$(RM) $(EXE)

install:
	mkdir -p "$(prefix_bin)" "$(prefix_images)"
	sudo install "$(EXE)" "$(prefix_bin)"
	sudo install "$(ICON)" "$(prefix_images)"

