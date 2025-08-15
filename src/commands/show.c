#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>             // For PATH_MAX
#include "../../include/util.h" // for get_local_dir()
#include "../../include/installer.h"
#include "../../include/parser.h"
#include "../../include/commands.h"

int has_installed_suffix(const char *filename)
{
    size_t len = strlen(filename);
    const char *suffix = "-installed.json";
    size_t suffix_len = strlen(suffix);
    return len > suffix_len && strcmp(filename + len - suffix_len, suffix) == 0;
}

int show_environments()
{
    char local_dir[PATH_MAX];
    get_local_dir(local_dir, sizeof(local_dir));

    DIR *dir = opendir(local_dir);
    if (!dir)
    {
        perror("[ERROR] Could not open virt-pack directory");
        return -1;
    }

    struct dirent *entry;
    int count = 0;

    printf("Environments:\n");

    while ((entry = readdir(dir)) != NULL)
    {
        const char *name = entry->d_name;

        // Skip "." and ".."
        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            continue;

        // Match files that end in "-installed.json"
        if (has_installed_suffix(name))
        {
            size_t len = strlen(name) - strlen("-installed.json");
            char env_name[256];
            strncpy(env_name, name, len);
            env_name[len] = '\0';

            printf("  %s\n", env_name);
            count++;
        }
    }

    closedir(dir);

    if (count == 0)
        printf("  (No environments found)\n");

    return 0;
}

int show_env_packages(const char *env_name)
{
    char local_dir[PATH_MAX];
    get_local_dir(local_dir, sizeof(local_dir));

    DIR *dir = opendir(local_dir);
    if (!dir)
    {
        perror("[ERROR] Could not open virt-pack directory");
        return -1;
    }

    struct dirent *entry;
    char target_file[PATH_MAX] = "";
    size_t env_len = strlen(env_name);

    while ((entry = readdir(dir)) != NULL)
    {
        const char *name = entry->d_name;
        size_t len = strlen(name);

        // Match files like "<env_name>-installed.json"
        if (len > 17 && strcmp(name + len - 17, "-installed.json") == 0 &&
            strncmp(name, env_name, env_len) == 0 && name[env_len] == '-')
        {
            snprintf(target_file, sizeof(target_file), "%s/%s", local_dir, name);
            break;
        }
    }

    closedir(dir);

    if (strlen(target_file) == 0)
    {
        fprintf(stderr, "[ERROR] Environment '%s' not found.\n", env_name);
        return -1;
    }

    // Load and parse the JSON file
    json_error_t error;
    json_t *root = json_load_file(target_file, 0, &error);
    if (!root || !json_is_array(root))
    {
        fprintf(stderr, "[ERROR] Failed to parse %s: %s\n", target_file, error.text);
        return -1;
    }

    printf("Packages in environment '%s':\n", env_name);
    size_t i;
    json_t *pkg;
    json_array_foreach(root, i, pkg)
    {
        if (json_is_string(pkg))
        {
            printf("  - %s\n", json_string_value(pkg));
        }
    }

    json_decref(root);
    return 0;
}
