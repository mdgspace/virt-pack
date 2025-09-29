#ifndef COMMANDS_H
#define COMMANDS_H

#define VIRT_PACK_LOCAL_DIR_PATH ".local/share/virt-pack"
#define PATH_MAX 4096

#define SCRIPTDIR PREFIX "/share/virt-pack/scripts"

enum pkg_manager {
    NONE, APT, XBPS
};

int handle_make(int argc, char *argv[], const char *);
int handle_remove(const char *);

#endif
