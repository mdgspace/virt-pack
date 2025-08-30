#ifndef COMMANDS_H
#define COMMANDS_H

#define VIRT_PACK_LOCAL_DIR_PATH ".local/share/virt-pack"
#define PATH_MAX 4096

void handle_make(int argc, char *argv[]);
void handle_remove(int argc, char *argv[]);
int show_environments();
int show_env_packages(const char *env_name);
void print_help();

#endif
