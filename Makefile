CC = gcc
CFLAGS = -Wall -Iinclude
SRC = src/main.c src/installer.c src/parser.c src/resolver.c src/uninstaller.c src/util.c src/commands/help.c src/commands/make.c src/commands/remove.c src/commands/show.c 
BIN = virt-pack # the binary executable
LIBS = -ljansson

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $(BIN) $(SRC) $(LIBS)

clean:
	rm -f $(BIN)
