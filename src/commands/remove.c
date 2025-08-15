#include "../../include/commands.h"
#include "../../include/util.h"

void handle_remove(int argc, char *argv[])
{
    // get env name
    char env_name[256];
    snprintf(env_name, sizeof(env_name), "%s", argv[2]);

    uninstaller_main(env_name);
}