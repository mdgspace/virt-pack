#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include <stdbool.h>

#include "../include/util.h"

#define PATH_MAX 4096

// util: check if a string is a flag
bool is_flag(const char *s)
{
    return strncmp(s, "--", 2) == 0;
}

// parse events.jsonl and collect libraries passed to pkg-config
json_t *collect_pkg_config_libs(const char *json_path)
{
    FILE *fp = fopen(json_path, "r");
    if (!fp)
    {
        perror("failed to open input json file");
        return NULL;
    }

    char buffer[65536];
    size_t line = 0;
    json_error_t error;
    json_t *lib_array = json_array();

    while (fgets(buffer, sizeof(buffer), fp))
    {
        line++;

        json_t *event = json_loads(buffer, 0, &error);
        if (!event)
        {
            fprintf(stderr, "JSON parse error on line %zu: %s\n", line, error.text);
            continue;
        }

        json_t *started = json_object_get(event, "started");
        if (!started)
        {
            json_decref(event);
            continue;
        }

        json_t *execution = json_object_get(started, "execution");
        if (!execution)
        {
            json_decref(event);
            continue;
        }

        json_t *exe = json_object_get(execution, "executable");
        if (!exe || !json_is_string(exe))
        {
            json_decref(event);
            continue;
        }

        const char *exe_str = json_string_value(exe);
        if (!strstr(exe_str, "pkg-config"))
        {
            json_decref(event);
            continue;
        }

        json_t *args = json_object_get(execution, "arguments");
        if (!args || !json_is_array(args))
        {
            json_decref(event);
            continue;
        }

        json_t *arg;
        size_t i;
        json_array_foreach(args, i, arg)
        {
            if (!json_is_string(arg))
                continue;
            const char *arg_str = json_string_value(arg);

            // skip the first token (pkg-config) and any flags
            if (i == 0 || is_flag(arg_str))
                continue;

            // append to lib_array
            // split arg_str by spaces to handle multiple libs in one argument
            char *arg_copy = strdup(arg_str);
            char *token = strtok(arg_copy, " ");
            while (token)
            {
                json_array_append_new(lib_array, json_string(token));
                token = strtok(NULL, " ");
            }
            free(arg_copy);
        }

        json_decref(event);
    }

    fclose(fp);
    return lib_array;
}

// write JSON array to output file
bool write_json_to_file(json_t *array, const char *filename)
{
    FILE *fp = fopen(filename, "w");
    if (!fp)
    {
        perror("failed to open output file");
        return false;
    }

    int result = json_dumpf(array, fp, JSON_INDENT(2));
    fclose(fp);
    return result == 0;
}

int parser_main(const char *env_name)
{
    printf("(*) parser main started\n");

    // get local virt-pack directory
    char local_dir[PATH_MAX];
    get_local_dir(local_dir, sizeof(local_dir));

    // construct path to the env-specific events.json file
    char input_file[PATH_MAX];
    snprintf(input_file, sizeof(input_file), "%s/%s-events.json", local_dir, env_name);

    // get libs-from-intercept.json <- output
    char output_file[PATH_MAX];
    snprintf(output_file, sizeof(output_file), "%s/libs-from-intercept.json", local_dir);

    // give events.jsonl as input to find out all required libraries
    json_t *libs = collect_pkg_config_libs(input_file);
    if (!libs)
        return EXIT_FAILURE;

    // write the names of required libraries to libs-from-intercept.json
    if (!write_json_to_file(libs, output_file))
    {
        json_decref(libs);
        return EXIT_FAILURE;
    }

    json_decref(libs);

    printf("(*) parser main ended\n");
    return EXIT_SUCCESS;
}