CC = gcc
TARGET = virt-pack-test-make

# Replace this with the libraries you want to use
LIB_NAME = <library-name>
PKG := pkg-config --cflags --libs $(LIB_NAME)

all: $(TARGET)

$(TARGET): main.c
	$(CC) main.c -o $(TARGET) $(shell $(PKG))

clean:
	rm -f $(TARGET)