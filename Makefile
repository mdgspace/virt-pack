CC := cc
SRC := src/main.c src/make.c
OBJS := $(SRC:.c=.o)
LDLIBS := -lbsd

PREFIX ?= /usr/local
CFLAGS += -DPREFIX=\"$(PREFIX)\"
BINDIR := $(PREFIX)/bin
SCRIPTDIR := $(PREFIX)/share/virt-pack

all: virt-pack

virt-pack: $(OBJS)
	$(CC) -o $@ $^ $(LDLIBS) $(CFLAGS)

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
