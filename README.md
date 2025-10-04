# virt-pack
# This is the readme file
virt-pack is a tool that uses [bear](https://github.com/rizsotto/Bear) to intercept your build commands, and detects missing system libraries (via `pkg-config`) and installs them using your system's package manager. It's especially useful for C/C++ projects with messy dependencies.

**Note:** This tool only supports dependencies using pkg-config.

## Installation
This version of virt-pack currently only supports apt and xbps package managers as it uses the package manager specific tools to install missing libraries.
Since the tool uses [bear](https://github.com/rizsotto/Bear) and [fzf] (https://github.com/junegunn/fzf), so install them via your respective package managers.
```
git clone https://github.com/mdgspace/dependency-manager
cd dependency-manager
sudo make install
```

## How to use
Then, move into your project directory with dependencies (should be calls to pkg-config) and run the following command:
```
virt-pack -- build_command
```

Keep in mind that the dependencies(should be calls to pkg-config) should be included in the Makefile / CmakeLists.txt. An example Makefile is provided below.
For more options you can pass `--help` parameter. 

To remove the installed packages when you don't need them anymore, run the following command:
```
virt-pack --remove
```
This will only remove the packages that were installed using virt-pack. The libraries that you had before will not be removed.

## Example

Here's a minimal Makefile that virt-pack can work with:
```
CC = gcc
TARGET = virt-pack-test

# Replace this with the libraries you want to use
LIB_NAME = <library-name>
PKG := pkg-config --cflags --libs $(LIB_NAME)

all: $(TARGET)

$(TARGET): main.c
	$(CC) main.c -o $(TARGET) $(shell $(PKG))

clean:
	rm -f $(TARGET)
```
See the [test/Makefile](test/Makefile) for a sample project that uses virt-pack.
