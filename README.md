# virt-pack
# This is the readme file
virt-pack is a tool that uses [bear](https://github.com/rizsotto/Bear) to intercept your build commands, and detects missing system libraries (via `pkg-config`) and installs them using your system's package manager. It's especially useful for C/C++ projects with messy dependencies.

**Note:** This version currently supports only Makefile and `CMakeLists.txt`-based projects.
Support for other build systems like Cargo is a work in progress.

## Installation

This version of virt-pack currently only supports Debian-based systems as it uses the `apt` and `apt-file` tools to install missing libraries.
Since the tool uses [bear](https://github.com/rizsotto/Bear), run the following command in your terminal to install it
```
sudo apt install bear
```
After this, we can run the following commands in an empty directory to install **virt-pack**
```
git clone https://github.com/ava-xmas/virt-pack-ubuntu.git
cd virt-pack-ubuntu
chmod +x install.sh
./install.sh
```

## How to use
Then, move into your project directory with missing dependencies and run the following command:
#### For `Makefile`-based projects
```
virt-pack make <env-name>
```
### For `CmakeLists.txt`-based projects
```
mkdir build
cd build
virt-pack cmake <env-name>
```

Keep in mind that the dependencies should be included in the Makefile / CmakeLists.txt. An example Makefile is provided below. Also, an `events.json` file will be generated in the directory where the command is run from. After virt-pack exits successfully, it is safe to delete that file.
For more options you can pass `--help` parameter. 

To remove the installed packages when you don't need them anymore, run the following command:
```
virt-pack remove <env-name>
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

## Working

### Database of Libraries
`virt-pack` maintains a local database (`virt-pack-db.json`) that maps library names (from `.pc` files) to their corresponding Debian packages.
To keep this mapping up to date, it uses a script that downloads Debian's [`Contents-amd64.gz`](http://ftp.debian.org/debian/dists/bookworm/main/Contents-amd64.gz), a list of all files in Debian packages.

### `virt-pack make` command
#### Parsing `bear intercept`
Upon running the make command, `bear intercept -- make` is run, which intercepts calls, including the ones made to `pkg-config`. These events are logged in `<env-name>-events.json`.

#### Resolving system libraries
The system libraries mentioned in the `pkg-config` calls (`.pc` files) are logged in `libs-from-intercept.json`. Then, the existance of each library on your system is checked using `pkg-config --exists [library-name]`. The missing libraries are logged to `missing-libs.json`

#### Installing required packages
The package name for the missing `.pc` files is found using the database. Then, `sudo apt install [package-name]` is run to install the packages. These are also logged to `<env-name>-installed.json`.

#### Uninstalling previously installed packages
`<env-name>-installed.json` file is used to uninstall the packages using `sudo apt remove -y [package-name]`.
After successful uninstallation, the `<env-name>-installed.json` file is also removed.