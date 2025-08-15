#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>

#define VIRT_PACK_LOCAL_DIR_PATH ".local/share/virt-pack"

// get the data directory to store files
void get_local_dir(char *out, size_t size)
{
    const char *home = getenv("HOME");
    if (!home)
    {
        fprintf(stderr, "[ERROR] HOME not set.\n");
        return; // exit(1)
    }

    snprintf(out, size, "%s/%s", home, VIRT_PACK_LOCAL_DIR_PATH);

    // create the dir if it doesnt exist
    struct stat st = {0};
    if (stat(out, &st) == -1)
    {
        mkdir(out, 0755);
    }
}
