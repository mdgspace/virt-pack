#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h> // for basename
#include <unistd.h> // for getcwd
#include <getopt.h>
#include "../include/commands.h"

#define PATH_MAX 4096

void print_version();
void update_db();
void remove_lib();

// ~/.local/share/virt-pack for persistent files
// ~/.cache/virt-pack for non-critical logs or intermediate files

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        print_help();
    }
    static struct option longopts[] = {
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'v'},
        {"update-db", no_argument, NULL, 'u'},
        {"remove", required_argument, NULL, 'r'}, //! check
        {0, 0, 0, 0}};

    //! trying getopt may be temporary
    //!  multiple options can be called at once is that ok
    int option;
    int sep_index = -1;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--") == 0)
        {
            sep_index = i;
            break;
        }
    }
    int getopt_argc = (sep_index == -1) ? argc : sep_index;
    optind = 1;
    // todo add options and do long
    while ((option = getopt_long(getopt_argc, argv, "hvur:", longopts, NULL)) != -1)
    {
        switch (option)
        {
        case 'h':
            print_help();
            break;
        case 'v':
            print_version();
            break;
        case 'u':
            update_db();
            break;
        case 'r':
            remove_lib();
            break;
        default:
            printf("Command Not Found");
            break;
        }
    }
    if (sep_index != -1 && sep_index + 1 < argc)
    {
        printf("Forwarding args after -- ");
        handle_make(argc - sep_index - 1, &argv[sep_index + 1]); //! handle_make ko sort karo
    }
    // todo what if -1
    //! trial over
}
void print_version()
{
    const char *version_text =
        "virt-pack version 1.0.0\n";

    fputs(version_text, stdout);
}
void update_db()
{
    int result = system("python3 /usr/local/share/virt-pack/scripts/update_db.py");
    if (result != 0)
    {
        fprintf(stderr, "[ERROR] Failed to update database\n");
    }
}
//! handle removing
void remove_lib()
{
    handle_remove();
}