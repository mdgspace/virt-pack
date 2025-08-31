#include <stdio.h>
#include <stdlib.h>
#include <jansson.h>
#include "../include/util.h"
#include <string.h>

#define PATH_MAX 4096

// uninstall a package via apt
void apt_remove(const char *pkgname)
{
    printf("Uninstalling %s...\n", pkgname);

    char cmd[256];
    snprintf(cmd, sizeof(cmd), "sudo apt remove -y %s", pkgname);

    int ret = system(cmd);
    if (ret != 0)
    {
        fprintf(stderr, "[ERROR] Failed to uninstall package: %s\n", pkgname);
        return;
    }

    printf("[OK] %s uninstalled successfully.\n", pkgname);
}

int remove_pkg()
{
    char path[PATH_MAX];
    snprintf(path, sizeof(path), ".virt-pack");

    FILE *fp = fopen(path, "r");
    if (!fp)
    {
        fprintf(stderr, "[ERROR] No .virt-pack file found in this directory.\n");
        return EXIT_FAILURE;
    }

    char pkgname[256] = {0};
    if (!fgets(pkgname, sizeof(pkgname), fp))
    {
        fprintf(stderr, "[ERROR] Failed to read package name from .virt-pack\n");
        fclose(fp);
        return EXIT_FAILURE;
    }
    fclose(fp);

    // strip trailing newline
    pkgname[strcspn(pkgname, "\n")] = 0;

    if (strlen(pkgname) == 0)
    {
        fprintf(stderr, "[ERROR] Empty package name in .virt-pack\n");
        return EXIT_FAILURE;
    }

    apt_remove(pkgname);

    // remove the .virt-pack file
    if (remove(path) != 0)
    {
        fprintf(stderr, "[WARNING] Failed to remove %s\n", path);
    }
    else
    {
        printf("[OK] Removed %s\n", path);
    }

    return EXIT_SUCCESS;
}
