#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h> // for basename
#include <unistd.h> // for getcwd

#include "../include/installer.h"
#include "../include/parser.h"
#include "../include/resolver.h"
#include "../include/uninstaller.h"
#include "../include/util.h"
#include "../include/commands.h"

#define PATH_MAX 4096

void print_version();

Command
parse_command(int argc, char *argv[])
{
    char *cmd = argv[1];
    char *env_name = "";

    if (argc > 2)
        env_name = argv[2];

    // build commands
    if (strcmp(cmd, "make") == 0)
        return CMD_BUILD;
    if (strcmp(cmd, "cmake") == 0)
        return CMD_BUILD;

    // removing packages
    if (strcmp(cmd, "remove") == 0)
        return CMD_REMOVE;

    // showing contents of env
    if (strcmp(cmd, "show") == 0)
    {
        if (strcmp(env_name, "") == 0)
            return CMD_SHOW;
        else
            return CMD_SHOW_ENV;
    }

    // updating the database
    if (strcmp(cmd, "update-db") == 0)
        return CMD_UPDATE_DB;

    // showing the help command
    if (strcmp(cmd, "--help") == 0 || strcmp(cmd, "help") == 0)
        return CMD_HELP;

    // showing the version
    if (strcmp(cmd, "--version") == 0 || strcmp(cmd, "version") == 0)
        return CMD_VERSION;
    return CMD_UNKNOWN;
}

BuildTool detect_build_command(int argc, char *argv[])
{
    char *tool = argv[1];

    if (strcmp(tool, "make") == 0)
    {
        return MAKE;
    }

    if (strcmp(tool, "cmake") == 0)
    {
        return CMAKE;
    }

    if (strcmp(tool, "cargo") == 0)
    {
        return CARGO;
    }

    return UNKNOWN;
}

// ~/.local/share/virt-pack for persistent files
// ~/.cache/virt-pack for non-critical logs or intermediate files

int main(int argc, char *argv[])
{

    Command cmd = parse_command(argc, argv);

    switch (cmd)
    {
    case CMD_HELP:
        print_help();
        break;

    case CMD_VERSION:
        print_version();
        break;

    case CMD_BUILD:
    {
        char env_name[256];

        if (argc >= 3)
        {
            snprintf(env_name, sizeof(env_name), "%s", argv[2]);
        }
        else
        {
            printf("Usage: virt-pack make <env-name>");
            return 1;
        }

        // detecting which build command is being used
        BuildTool tool = detect_build_command(argc, argv);

        if (tool == UNKNOWN)
        {
            printf("Build tool not recognized.\n");
            return 1;
        }

        if (tool == CARGO)
        {
            printf("Build tool in development.\n");
            return 1;
        }

        // logic to run parser -> resolver -> installer whilst referencing events.jsonl from the user's project directory
        // ( after running bear intercept -- make )

        switch (tool)
        {
        case MAKE:
        {
            handle_make(argc, argv, tool);
            break;
        }

        case CMAKE:
        {
            handle_make(argc, argv, tool);
            break;
        }
        }
        break;
    }

    case CMD_REMOVE:
    {
        char env_name[256]; // buffer to hold the directory path

        if (argc >= 3)
        {
            snprintf(env_name, sizeof(env_name), "%s", argv[2]);
        }
        else
        {
            printf("Usage: virt-pack remove <env-name>\n");
            printf("Use `virt-pack show` to show names of all environments.\n");
            return 1;
        }
        handle_remove(argc, argv);
        break;
    }

    case CMD_SHOW:
    {
        if (show_environments() < 0)
            return 1;
        break;
    }

    case CMD_SHOW_ENV:
    {
        char env_name[256];

        if (argc >= 3)
        {
            snprintf(env_name, sizeof(env_name), "%s", argv[2]);
        }

        show_env_packages(env_name);
        break;
    }

    case CMD_UPDATE_DB:
    {
        int result = system("python3 /usr/local/share/virt-pack/scripts/update_db.py");
        if (result != 0)
        {
            fprintf(stderr, "[ERROR] Failed to update database\n");
        }
        break;
    }

    case CMD_UNKNOWN:
    {
        printf("Command not recognized. Use `virt-pack help` to see valid commands.");
        break;
    }
    }
}

void print_version()
{
    const char *version_text =
        "virt-pack version 1.0.0\n";

    fputs(version_text, stdout);
}
