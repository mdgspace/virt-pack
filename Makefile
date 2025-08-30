CC := cc
SRC := src/main.c src/installer.c src/parser.c src/resolver.c src/uninstaller.c src/util.c src/make.c src/show.c
OBJS := $(SRC:.c=.o)
LDLIBS := -ljansson

PREFIX ?= /usr/local
BINDIR := $(PREFIX)/bin
SCRIPTDIR := $(PREFIX)/share/virt-pack

all: virt-pack

virt-pack: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

install: all
	mkdir -p $(BINDIR) $(SCRIPTDIR)
	install virt-pack $(BINDIR)
	cp -r scripts/update_db.py $(SCRIPTDIR)
	chmod -R 755 $(SCRIPTDIR)/scripts

clean:
	rm -f virt-pack src/*.o

