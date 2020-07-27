define colorecho
      @tput setaf 2
      @echo $1
      @tput sgr0
endef

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

testing: FORCE
	@echo "Testing"
	@echo $(LINUX_DIR)
	@echo $(ANDROID_DIR)
	@echo $(WINDOWS_DIR)
FORCE:

TARGETS     = 0 0a 0b 0c 0d 1 1a 1b 2 2a 2b 2c 2d 2e 3a 4 4a 4b 4c

.PHONY: all
all: $(TARGETS)

0:   $(SRCDIR)helper.c $(SRCDIR)0.c
	@cc $(CFLAGS) -o $(LINUX_DIR)$@ $+ $(LDFLAGS)
	$(call colorecho,"$@ success. ./\"$@\" to execute.")

0a:   $(SRCDIR)helper_0.c $(SRCDIR)0a.c
	@cc $(CFLAGS) -o $(LINUX_DIR)$@ $+ $(LDFLAGS)
	$(call colorecho,"$@ success. ./\"$@\" to execute.")

0b:   $(SRCDIR)helper_0.c $(SRCDIR)0b.c
	@cc $(CFLAGS) -o $(LINUX_DIR)$@ $+ $(LDFLAGS)
	$(call colorecho,"$@ success. ./\"$@\" to execute.")

0c:   $(SRCDIR)helper_0.c $(SRCDIR)0c.c
	@cc $(CFLAGS) -o $(LINUX_DIR)$@ $+ $(LDFLAGS)
	$(call colorecho,"$@ success. ./\"$@\" to execute.")

0d:   $(SRCDIR)helper_0.c $(SRCDIR)0d.c
	@cc $(CFLAGS) -o $(LINUX_DIR)$@ $+ $(LDFLAGS)
	$(call colorecho,"$@ success. ./\"$@\" to execute.")



1:   $(SRCDIR)helper_0.c $(SRCDIR)1.c
	@cc $(CFLAGS) -o $(LINUX_DIR)$@ $+ $(LDFLAGS)
	$(call colorecho,"$@ success. ./\"$@\" to execute.")

1a:   $(SRCDIR)helper_0.c $(SRCDIR)1a.c
	cc $(CFLAGS) -o $(LINUX_DIR)$@ $+ $(LDFLAGS)
	$(call colorecho,"$@ success. ./\"$@\" to execute.")

1b:   $(SRCDIR)helper_0.c $(SRCDIR)1b.c
	cc $(CFLAGS) -o $(LINUX_DIR)$@ $+ $(LDFLAGS)
	$(call colorecho,"$@ success. ./\"$@\" to execute.")



2:   $(SRCDIR)helper_0.c $(SRCDIR)2.c
	cc $(CFLAGS) -o $(LINUX_DIR)$@ $+ $(LDFLAGS)
	$(call colorecho,"$@ success. ./\"$@\" to execute.")

2a:   $(SRCDIR)helper_0.c $(SRCDIR)2a.c
	cc $(CFLAGS) -o $(LINUX_DIR)$@ $+ $(LDFLAGS)
	$(call colorecho,"$@ success. ./\"$@\" to execute.")

2b:   $(SRCDIR)helper_0.c $(SRCDIR)2b.c
	cc $(CFLAGS) -o $(LINUX_DIR)$@ $+ $(LDFLAGS)
	$(call colorecho,"$@ success. ./\"$@\" to execute.")

2c:   $(SRCDIR)helper_0.c $(SRCDIR)2c.c
	cc $(CFLAGS) -o $(LINUX_DIR)$@ $+ $(LDFLAGS)
	$(call colorecho,"$@ success. ./\"$@\" to execute.")

2d:   $(SRCDIR)helper_0.c $(SRCDIR)2d.c
	cc $(CFLAGS) -o $(LINUX_DIR)$@ $+ $(LDFLAGS)
	$(call colorecho,"$@ success. ./\"$@\" to execute.")

2e:   $(SRCDIR)helper_0.c $(SRCDIR)2e.c
	cc $(CFLAGS) -o $(LINUX_DIR)$@ $+ $(LDFLAGS)
	$(call colorecho,"$@ success. ./\"$@\" to execute.")


3:   $(SRCDIR)helper_0.c $(SRCDIR)3.c
	cc $(CFLAGS) -o $(LINUX_DIR)$@ $+ $(LDFLAGS)
	$(call colorecho,"$@ success. ./\"$@\" to execute.")

3a:   $(SRCDIR)helper_0.c $(SRCDIR)3a.c
	cc $(CFLAGS) -o $(LINUX_DIR)$@ $+ $(LDFLAGS)
	$(call colorecho,"$@ success. ./\"$@\" to execute.")



4:   $(SRCDIR)helper_0.c $(SRCDIR)4.c
	cc $(CFLAGS) -o $(LINUX_DIR)$@ $+ $(LDFLAGS)
	$(call colorecho,"$@ success. ./\"$@\" to execute.")

4a:   $(SRCDIR)helper_0.c $(SRCDIR)4a.c
	cc $(CFLAGS) -o $(LINUX_DIR)$@ $+ $(LDFLAGS)
	$(call colorecho,"$@ success. ./\"$@\" to execute.")

4b:   $(SRCDIR)helper_0.c $(SRCDIR)4b.c
	cc $(CFLAGS) -o $(LINUX_DIR)$@ $+ $(LDFLAGS)
	$(call colorecho,"$@ success. ./\"$@\" to execute.")

4c:   $(SRCDIR)helper_0.c $(SRCDIR)4c.c
	cc $(CFLAGS) -o $(LINUX_DIR)$@ $+ $(LDFLAGS)
	$(call colorecho,"$@ success. ./\"$@\" to execute.")

.PHONY: clean
clean:
	@cd $(LINUX_DIR) && rm $(TARGETS) 2>/dev/null || true

# hot compile
# while inotifywait -e close_write ./code.c; do date +%X; make > /dev/null; done

#$<	first dependency
#$@	target name
#$+	list of all dependencies
#$^	list of all unique dependencies
