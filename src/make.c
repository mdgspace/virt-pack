#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include "commands.h"
#define PATH_MAX 4096

char *join_args(int argc, char *argv[]);
void bear_intercept(int argc, char *argv[])
{
    printf("Intercepting using bear...\n");

    char cmd[1024] = SCRIPTDIR "/bear-intercept.sh ";
    for (int i = 0; i < argc; i++) {
        strlcat(cmd, argv[i], sizeof(cmd));
        if (i < argc - 1) {
            strlcat(cmd, " ", sizeof(cmd));
        }
    }

    printf("Running: %s\n",cmd);
    int ret = system(cmd);
    if (ret != 0)
    {
        fprintf(stderr, "[ERROR] Failed to run bear intercept command\n");
        return;
    }
}

int handle_make(int argc, char *argv[], const char *pkgmgr)
{
    bear_intercept(argc, argv);
    // parser_main();

    char installer[1024];
    snprintf(installer, 1024, SCRIPTDIR "/%s-install.sh", pkgmgr);
	execl(installer, "installer", (char*)NULL);

    return 0;
}

int handle_remove(const char *pkgmgr)
{
    char uninstaller[256];
    snprintf(uninstaller, 256, SCRIPTDIR "/%s-uninstall.sh", pkgmgr);
    execl(uninstaller, "uninstaller", (char *)NULL);
    return 0;
}

int info_package(const char *pkgmgr) {
    char info[256];
    snprintf(info, 256, SCRIPTDIR "/%s-info.sh", pkgmgr);
    execl(info, "info", (char *)NULL);
    return 0;
}
