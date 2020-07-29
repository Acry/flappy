## Tool chain

- OS: Arch Linux - Rolling Releases
- UI: Gnome
- Shell: Bash
- IDE: VS Code
- CC: GNU GCC
- Build system: GNU Make
- Debugger: gdb with UI: Nemiver
- Testing: tbd

## Testing step

To test the tool-chain type `make 0`, change into `./Builds/Linux` and run `0` or use your file-browser and you should see following screen.

![](Images/Screenshot_0.png)

If everything runs like expected you can move on, else try to fix it yourself. It is crucial to have everything until now under control. Later, as things become more complex, it may be very hard to gain control back.

## Install SDL2

The code is linking against: `-lSDL2_image -lSDL2_mixer`<br>

arch-based distributions:<br>

`sudo pacman -S sdl2 sdl2_image sdl2_mixer`

If you want all libs to experiment with them, use:<br>
`sudo pacman -S sdl2 sdl2_image sdl2_mixer sdl2_gfx sdl2_net sdl2_ttf`

Gentoo users can "sudo emerge libsdl2" to get everything they need.<br>

On other distros look out for sdl2 and their developer packages:<br>

debian packages:<br>
deb: libsdl2,libsdl2-dev<br>

red hat packages:<br>
rpm: SDL2 SDL2-devel<br>

## Skeleton

The skeleton is not really necessary anymore. I was using my KDevelop Light Template for Prototyping.

You can grab it here: <https://store.kde.org/p/1234090/>, here: <https://github.com/Acry/SDL2-C-KDev_App_Template_light> or directly in KDevelop.

## Makefile

Function that calls `tput` for colored output with `make`<br>
See https://unix.stackexchange.com/a/269085/230528 for more info on that.

```shell
define colorecho
      @tput setaf 2
      @echo $1
      @tput sgr0
endef

$(call colorecho,"$@ success. ./\"$@\" to execute.")
```

```make
CFLAGS     = -no-pie -ggdb -Wall -Wextra -mtune=native `sdl2-config --cflags`
LDFLAGS    = `sdl2-config --libs` -lSDL2_image -lSDL2_mixer -lm

SRCDIR	    = Code/
BUILDDIR    = Builds/
WINDOWS     = Windows/
WINDOWS_DIR = $(BUILDDIR)$(WINDOWS)
ANDROID     = Android/
ANDROID_DIR = $(BUILDDIR)$(ANDROID)
LINUX       = Linux/
LINUX_DIR   = $(BUILDDIR)$(LINUX)

TARGETS     = 0

.PHONY: all
all: $(TARGETS)

0:   $(SRCDIR)helper.c $(SRCDIR)0.c
	@cc $(CFLAGS) -o $(LINUX_DIR)$@ $+ $(LDFLAGS)


.PHONY: clean
clean:
	@rm $(TARGETS) 2>/dev/null || true
```
