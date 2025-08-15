#ifndef COMMANDS_H
#define COMMANDS_H

#define VIRT_PACK_LOCAL_DIR_PATH ".local/share/virt-pack"
#define PATH_MAX 4096

// to store which command is being called
typedef enum
{
    CMD_BUILD,
    CMD_REMOVE,
    CMD_SHOW,
    CMD_SHOW_ENV,
    CMD_UPDATE_DB,
    CMD_HELP,
    CMD_VERSION,
    CMD_UNKNOWN
} Command;

// to store the type of build tool being used
typedef enum
{
    MAKE,
    CMAKE,
    CARGO,
    UNKNOWN,
} BuildTool;

void handle_make(int argc, char *argv[], BuildTool tool);
void handle_remove(int argc, char *argv[]);
int show_environments();
int show_env_packages(const char *env_name);
void print_help();

#endif