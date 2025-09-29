#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <jansson.h>
#include <stdbool.h>
#include "commands.h"
#define PATH_MAX 4096

char *join_args(int argc, char *argv[]); 
void parse_libs(char **libs) {
    fprintf(stderr, "(*) parse_libs started\n");
    FILE *events = fopen("events.json", "r");
    char buffer[65536];
    size_t line = 0;
    json_error_t error;
    json_t *lib_array = json_array();
    int index = 1;

    while (fgets(buffer, sizeof(buffer), events))
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

            int cont = 0;
            if (!json_is_string(arg))
                continue;

            const char *arg_str = json_string_value(arg);

            // skip the first token (pkg-config) and any flags
            if (i == 0 || strncmp(arg_str, "--", 2))
                continue;

            for (int j = 1; j < index; j++) {
                if (strcmp(libs[j], arg_str) == 0)
                    cont = 1;
            }

            if (cont)
                continue;

            // append to lib_array
            // split arg_str by spaces to handle multiple libs in one argument
            // char *arg_copy = strdup(arg_str);
            libs[index] = strdup(arg_str);
            fprintf(stderr, "%s:%d:%s: lib: %s\n", __FILE__, __LINE__, __func__, libs[index]);
            index++;
        }

        json_decref(event);
    }
    libs[index] = NULL;
}

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

int info_package(const char *pkgmgr) {
    char info[256];
    snprintf(info, 256, SCRIPTDIR "/%s-info.sh", pkgmgr);
    execl(info, "info", (char *)NULL);
    return 0;
}
