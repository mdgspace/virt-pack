#include <stdio.h>
#include <stdlib.h>
#include <jansson.h>
#include <string.h>

#include "../include/util.h"

#define PATH_MAX 4096

// install a package via apt
void install_package(const char *pkgname)
{
    printf("Installing %s\n...", pkgname);

    // apt command
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "sudo apt install -y %s", pkgname);

    int ret = system(cmd);
    if (ret != 0)
    {
        fprintf(stderr, "[ERROR] Failed to install package: %s\n", pkgname);
    }
}

// add package name to installed.json env record
void record_installed_packages(json_t *installed_map, const char *env_name, const char *pkgname)
{

    json_t *pkg_array = json_object_get(installed_map, env_name);

    if (!pkg_array)
    {
        pkg_array = json_array();
        json_object_set_new(installed_map, env_name, pkg_array);
    }

    // add only if not already present
    size_t i;
    json_t *val;
    json_array_foreach(pkg_array, i, val)
    {
        if (json_is_string(val) && strcmp(json_string_value(val), pkgname) == 0)
            return;
    }

    json_array_append_new(pkg_array, json_string(pkgname));
}

int installer_main(const char *env_name)
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

    // install the packages

    char installed_file[PATH_MAX];
    snprintf(installed_file, sizeof(installed_file), "%s/%s-installed.json", local_dir, env_name);

    json_t *installed = json_load_file(installed_file, 0, &error);
    if (!installed)
        installed = json_object();

    size_t i;
    json_t *lib_name;
    // get the array of packages corr to each lib name
    json_array_foreach(missing_libs, i, lib_name)
    {
        if (!json_is_string(lib_name))
            continue;

        const char *name = json_string_value(lib_name);
        json_t *pkgs = json_object_get(db, name);

        if (!pkgs || !json_is_array(pkgs))
        {
            fprintf(stderr, "[WARNING] No packages found for %s\n", name);
            continue;
        }

        // get the first package in the array
        const char *pkg_path = json_string_value(json_array_get(pkgs, 0));

        // extract the debian package name from path
        const char *slash = strchr(pkg_path, '/');
        const char *pkg = slash ? slash + 1 : pkg_path;

        install_package(pkg);
        record_installed_packages(installed, env_name, pkg);
    }

    json_decref(missing_libs);
    json_decref(db);

    if (json_dump_file(installed, installed_file, JSON_INDENT(2)) != 0)
    {
        fprintf(stderr, "[ERROR] Failed to write to %s\n", installed_file);
        json_decref(installed);
        return EXIT_FAILURE;
    }

    json_decref(installed);

    printf("(*) installer main ended\n");
    return 0;
}