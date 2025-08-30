#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <jansson.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "util.h"

#define PATH_MAX 4096

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
    //!What is virt_pack_db.json
    //! can probably remove installed_file
    // load virt-pack-db.json -> object
    char db_file[PATH_MAX];
    snprintf(db_file, sizeof(db_file), "%s/virt-pack-db.json", local_dir);

    json_t *db = json_load_file(db_file, 0, &error);
    if (!db || !json_is_object(db))
    {
        fprintf(stderr, "[ERROR] Failed to load %s: %s\n", db_file, error.text);
        json_decref(missing_libs);
        return EXIT_FAILURE;
    }

    size_t i;
    json_t *lib_name;
    size_t dep_count = json_array_size(missing_libs);
    char **args = malloc((dep_count + 3) * sizeof(char*)); 
    if (!args) {
    fprintf(stderr, "[ERROR] malloc failed\n");
    json_decref(missing_libs);
    json_decref(db);
    return EXIT_FAILURE;
    }
    //! currently 2hardcoded for ubuntu
    char* pkg_name=basename(local_dir);
    //! basename may not be unique
    args[0] = "./deb_install.sh"; 
    args[1] = strdup(pkg_name);


    size_t arg_index = 2;
    // get the array of packages corr to each lib name
    json_array_foreach(missing_libs, i, lib_name)
    {
        if (!json_is_string(lib_name))
            continue;

        const char *name = json_string_value(lib_name);
        json_t *pkgs = json_object_get(db, name);

        if (!pkgs || !json_is_array(pkgs)||json_array_size(pkgs)==0)
        {
            fprintf(stderr, "[WARNING] No packages found for %s\n", name);
            continue;
        }

        // get the first package in the array
        const char *pkg_path = json_string_value(json_array_get(pkgs, 0));

        // extract the debian package name from path
        const char *slash = strchr(pkg_path, '/');
        const char *pkg = slash ? slash + 1 : pkg_path;
        //yaha pe start installing them
        args[arg_index++]=strdup(pkg);
        // install_package(pkg);
        // record_installed_packages(installed, env_name, pkg);
    }
    args[arg_index]=NULL;
    pid_t pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
        perror("execvp failed");
        _exit(127);
    } else if (pid < 0) {
    perror("fork failed");
    } else {
        int status;
        waitpid(pid, &status, 0);
    if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
        fprintf(stderr, "[ERROR] virtual package install failed\n");
        }
    }

// free args
for (size_t j = 1; j < arg_index; j++) {
    free(args[j]);
}
free(args);
    json_decref(missing_libs);
    json_decref(db);


    printf("(*) installer main ended\n");
    return 0;
}

