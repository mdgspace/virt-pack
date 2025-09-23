CC := cc
SRC := src/main.c src/installer.c src/parser.c src/resolver.c src/make.c src/show.c
OBJS := $(SRC:.c=.o)
LDLIBS := -ljansson

PREFIX ?= /usr/local
CFLAGS += -DPREFIX=\"$(PREFIX)\"
BINDIR := $(PREFIX)/bin
SCRIPTDIR := $(PREFIX)/share/virt-pack

all: virt-pack

virt-pack: $(OBJS)
	$(CC) -o $@ $^ $(LDLIBS) $(CFLAGS)
	# $(CC) $(CFLAGS) -o $@ $^ $(LDLIBS) -DPREFIX=\"$(PREFIX)\"

install: all
	mkdir -p $(BINDIR) $(SCRIPTDIR)
	install virt-pack $(BINDIR)
	cp -r scripts $(SCRIPTDIR)
	chmod -R 755 $(SCRIPTDIR)/scripts

uninstall:
	rm -f $(BINDIR)/virt-pack
	rm -rf $(SCRIPTDIR)

clean:
	rm -f virt-pack src/*.o

