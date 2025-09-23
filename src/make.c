#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "commands.h"
#include "parser.h"

char *join_args(int argc, char *argv[]); 

void bear_intercept(int argc, char *argv[])
{
    printf("Intercepting using bear...\n");

    char cmd[256] = "bear intercept -- ";
    for (int i = 0; i < argc; i++) {
        strlcat(cmd, argv[i], sizeof(cmd));
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
    char *libs[256];
    libs[0] = "installer";
    parse_libs(libs);

    char installer[1024];
    snprintf(installer, 1024, SCRIPTDIR "/%s-install.sh", pkgmgr); 


	if (execv(installer, libs)) {
    	perror("execv");
	}

    return 0;
}

int handle_remove(const char *pkgmgr)
{
    char uninstaller[256];
    snprintf(uninstaller, 256, SCRIPTDIR "/%s-uninstall.sh", pkgmgr);
    execl(uninstaller, "uninstaller", (char *)NULL);
    return 0;
}
