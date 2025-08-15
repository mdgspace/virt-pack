#include <stdio.h>
#include <stdlib.h>
#include <jansson.h>
#include <string.h>

#define VIRT_PACK_LOCAL_DIR_PATH ".local/share/virt-pack"

#define PATH_MAX 4096

// uninstall a package via apt
void uninstall_package(const char *pkgname)
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

int uninstaller_main(const char *env_name)
{
    char local_dir[PATH_MAX];
    get_local_dir(local_dir, sizeof(local_dir));

    char installed_file[PATH_MAX];
    snprintf(installed_file, sizeof(installed_file), "%s/%s-installed.json", local_dir, env_name);

    json_error_t error;
    json_t *pkg_array = json_load_file(installed_file, 0, &error);

    if (!pkg_array || !json_is_array(pkg_array))
    {
        fprintf(stderr, "[ERROR] Failed to load %s: %s\n", installed_file, error.text);
        return EXIT_FAILURE;
    }

    size_t i;
    json_t *pkg;
    json_array_foreach(pkg_array, i, pkg)
    {
        if (!json_is_string(pkg))
            continue;

        const char *pkg_name = json_string_value(pkg);
        uninstall_package(pkg_name);
    }

    if (remove(installed_file) != 0)
    {
        fprintf(stderr, "[WARNING] Failed to remove %s\n", installed_file);
    }
    else
    {
        printf("[OK] Removed %s\n", installed_file);
    }

    // delete the installed file after uninstall
    if (remove(installed_file) != 0)
    {
        fprintf(stderr, "[WARNING] Failed to remove %s\n", installed_file);
    }
    else
    {
        printf("[OK] Removed %s\n", installed_file);
    }

    json_decref(pkg_array);
    return EXIT_SUCCESS;
}
