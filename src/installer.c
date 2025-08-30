#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include "unistd.h"
#include <jansson.h>
#include <string.h>
#include "../include/util.h"
#include <sys/types.h>
#include <sys/wait.h>
#define PATH_MAX 4096

// helper to run apt-file search
char **apt_file_search(const char *lib, int *count)
{
    char cmd[PATH_MAX];
    snprintf(cmd, sizeof(cmd), "apt-file search -x '%s$'", lib);
    // using regex match flag

    FILE *fp = popen(cmd, "r");
    if (!fp)
    { // if the command fails
        perror("popen failed");
        *count = 0;
        return NULL;
    }

    char line[PATH_MAX];
    char **pkgs = NULL;
    int n = 0;
    while (fgets(line, sizeof(line), fp))
    {
        char *colon = strchr(line, ':');
        *colon = '\0';
        char *pkg = strdup(line);
        if (!pkg)
            continue;

        // check if already added
        int exists = 0;
        for (int i = 0; i < n; i++)
        {
            if (strcmp(pkgs[i], pkg) == 0)
            {
                exists = 1;
                break;
            }
        }

        if (!exists)
        {
            pkgs = realloc(pkgs, (n + 1) * sizeof(char *));
            pkgs[n++] = pkg;
        }
        else
        {
            free(pkg);
        }
    }
    pclose(fp);
    *count = n;
    return pkgs;
}

int installer_main()
{
    printf("(*) installer main started\n");

    json_error_t error;

    // get local directory path
    char local_dir[PATH_MAX];
    get_local_dir(local_dir, sizeof(local_dir));

    // load missing-libs.json -> array
    char missing_file[PATH_MAX];
    snprintf(missing_file, sizeof(missing_file), "%s/missing-libs.json", local_dir);

    json_t *missing_libs = json_load_file(missing_file, 0, &error);
    if (!missing_libs || !json_is_array(missing_libs))
    {
        fprintf(stderr, "[ERROR] Failed to load %s: %s\n", missing_file, error.text);
        return EXIT_FAILURE;
    }
    //! can probably remove installed_file

    size_t dep_count = json_array_size(missing_libs);
    char **args = malloc((dep_count + 3) * sizeof(char *));
    if (!args)
    {
        fprintf(stderr, "[ERROR] malloc failed\n");
        json_decref(missing_libs);

        return EXIT_FAILURE;
    }
    //! currently 2hardcoded for ubuntu
    char *pkg_name = basename(local_dir);
    //! basename may not be unique
    // char script_path[PATH_MAX];
    // snprintf(script_path, sizeof(script_path), "%s/deb_install.sh",local_dir);
    // printf(script_path);
    char exe_path[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
    if (len == -1)
    {
        perror("readlink");
        exit(1);
    }
    exe_path[len] = '\0';

    char *exe_dir = dirname(exe_path);

    char script_path[PATH_MAX];
    snprintf(script_path, sizeof(script_path), "%s/deb_install.sh", exe_dir);
    printf(script_path);
    args[0] = strdup(script_path);
    args[0] = script_path;
    args[1] = strdup(pkg_name);

    size_t arg_index = 2;

    size_t i;
    json_t *lib_name;

    // get the array of packages corr to each lib name
    json_array_foreach(missing_libs, i, lib_name)
    {
        if (!json_is_string(lib_name))
            continue;

        const char *name = json_string_value(lib_name);

        int count = 0;
        char **pkgs = apt_file_search(name, &count);
        if (count == 0)
        {
            fprintf(stderr, "[WARNING] No package found for %s\n", name);
            continue;
        }

        int choice = 0;
        if (count > 1)
        {
            printf("\nMultiple packages found for %s:\n", name);
            for (int j = 0; j < count; j++)
            {
                printf(" [%d] %s\n", j + 1, pkgs[j]);
            }
            printf("Select package [1-%d]: ", count);
            fflush(stdout);
            scanf("%d", &choice);
            if (choice < 1 || choice > count)
                choice = 1;
        }

        const char *pkg = pkgs[choice - 1];
        args[arg_index++] = strdup(pkg);

        for (int j = 0; j < count; j++)
            free(pkgs[j]);
        free(pkgs);
    }
    args[arg_index] = NULL;
    pid_t pid = fork();
    if (pid == 0)
    {
        execvp(args[0], args);
        perror("execvp failed");
        _exit(127);
    }
    else if (pid < 0)
    {
        perror("fork failed");
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        {
            fprintf(stderr, "[ERROR] virtual package install failed\n");
        }
    }

    // free args
    for (size_t j = 1; j < arg_index; j++)
    {
        free(args[j]);
    }
    free(args);
    json_decref(missing_libs);

    printf("(*) installer main ended\n");
    return 0;
}
