#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include <unistd.h>

#include "../include/util.h"

#define PATH_MAX 4096

// runs `pkg-config --exists <lib>` and checks return code ( to check for already existing libraries )
int is_installed(const char *lib_name)
{
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "pkg-config --exists %s", lib_name);
    int ret = system(cmd);
    return ret == 0;
}

int resolver_main()
{
    printf("(*) resolver main started\n");

    // get local directory path
    char local_dir[PATH_MAX];
    get_local_dir(local_dir, sizeof(local_dir));

    // get intercepted libraries list
    char intercepted_libs[PATH_MAX];
    snprintf(intercepted_libs, sizeof(intercepted_libs), "%s/libs-from-intercept.json", local_dir);

    FILE *fp = fopen(intercepted_libs, "r");
    if (!fp)
    {
        printf("[ERROR] Failed to open %s\n", intercepted_libs);
        return EXIT_FAILURE;
    }

    json_error_t error;
    json_t *lib_array = json_loadf(fp, 0, &error);
    fclose(fp);

    if (!json_is_array(lib_array))
    {
        fprintf(stderr, "Expected a JSON array in %s\n", intercepted_libs);
        json_decref(lib_array);
        return EXIT_FAILURE;
    }

    json_t *missing_array = json_array();

    size_t i;
    json_t *lib;
    json_array_foreach(lib_array, i, lib)
    {
        if (!json_is_string(lib))
            continue;

        const char *lib_name = json_string_value(lib);
        if (is_installed(lib_name))
        {
            printf("[OK] %s\n", lib_name);
        }
        else
        {
            printf("[MISSING] %s\n", lib_name);
            json_array_append_new(missing_array, json_string(lib_name));
        }
    }

    // get file to write missing libraries in
    char missing_libs[PATH_MAX];
    snprintf(missing_libs, sizeof(missing_libs), "%s/missing-libs.json", local_dir);

    fp = fopen(missing_libs, "w");
    if (!fp)
    {
        printf("Failed to open %s for writing", missing_libs);
        json_decref(lib_array);
        json_decref(missing_array);
        return EXIT_FAILURE;
    }

    json_dumpf(missing_array, fp, JSON_INDENT(2));
    fclose(fp);

    printf("\n%zu missing libraries written to %s\n", json_array_size(missing_array), missing_libs);

    json_decref(lib_array);
    json_decref(missing_array);

    printf("(*) resolver main ended\n");
    return EXIT_SUCCESS;
}
