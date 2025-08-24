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
// Command
// parse_command(int argc, char *argv[])
// {
//     char *cmd = argv[1];
//     char *env_name = "";
//     //!check this out
//     if (argc > 2)
//         env_name = argv[2];

//     // build commands
//     // if (strcmp(cmd, "make") == 0)
//     //     return CMD_BUILD;
//     // if (strcmp(cmd, "cmake") == 0)
//     //     return CMD_BUILD;

//     // removing packages
//     //!have to make a seperate script for this
//     if (strcmp(cmd, "remove") == 0)
//         return CMD_REMOVE;

//     // showing contents of env

//     // if (strcmp(cmd, "show") == 0)
//     // {
//     //     if (strcmp(env_name, "") == 0)
//     //         return CMD_SHOW;
//     //     else
//     //         return CMD_SHOW_ENV;
//     // }

//     // updating the database
//     if (strcmp(cmd,"--build")==0)
//         return CMD_BUILD;
//     if (strcmp(cmd, "update-db") == 0)
//         return CMD_UPDATE_DB;
//     // showing the help command
//     if (strcmp(cmd, "--help") == 0 || strcmp(cmd, "help") == 0)
//         return CMD_HELP;

//     // showing the version
//     if (strcmp(cmd, "--version") == 0 || strcmp(cmd, "version") == 0)
//         return CMD_VERSION;
//     return CMD_UNKNOWN;
// }
//! shouldn't need this anymore
// BuildTool detect_build_command(int argc, char *argv[])
// {
//     char *tool = argv[1];

//     if (strcmp(tool, "make") == 0)
//     {
//         return MAKE;
//     }

//     if (strcmp(tool, "cmake") == 0)
//     {
//         return CMAKE;
//     }

//     if (strcmp(tool, "cargo") == 0)
//     {
//         return CARGO;
//     }

//     return UNKNOWN;
// }

// ~/.local/share/virt-pack for persistent files
// ~/.cache/virt-pack for non-critical logs or intermediate files

int main(int argc, char *argv[])
{   
    if(argc<2){
        print_help(); 
    }
    static struct option longopts[]={
        {"help",no_argument,NULL,'h'},
        {"version",no_argument,NULL,'v'},
        {"update-db",no_argument,NULL,'u'},
        {"remove",required_argument,NULL,'r'},//!check
        {0,0,0,0}
    };
    //!trying getopt may be temporary
    //! multiple options can be called at once is that ok 
    int option;
    int sep_index=-1;
    for(int i=1;i<argc;i++){
        if(strcmp(argv[i],"--")==0){
            sep_index=i;
            break;
        }
    }
    int getopt_argc=(sep_index==-1)?argc:sep_index;
    optind=1;
    //todo add options and do long
    while((option=getopt_long(getopt_argc,argv,"hvur:",longopts,NULL))!=-1){
        switch (option){
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
    if(sep_index!=-1&&sep_index+1<argc){
        printf("Forwarding args after -- ");
        handle_make(argc-sep_index-1,&argv[sep_index+1]);//! handle_make ko sort karo
    }
    //todo what if -1
    //! trial over
    //Command cmd = parse_command(argc, argv);

    // switch (cmd)
    // {
    // case CMD_HELP:
    //     print_help();
    //     break;

    // case CMD_VERSION:
    //     print_version();
    //     break;

    // case CMD_BUILD:
    // {
    //     char env_name[256];

    //     if (argc >= 3)
    //     {
    //         snprintf(env_name, sizeof(env_name), "%s", argv[2]);
    //     }
    //     else
    //     {
    //         printf("Usage: virt-pack make <env-name>");
    //         return 1;
    //     }

    //     // detecting which build command is being used
    //     //BuildTool tool = detect_build_command(argc, argv);

    //     if (tool == UNKNOWN)
    //     {
    //         printf("Build tool not recognized.\n");
    //         return 1;
    //     }

    //     if (tool == CARGO)
    //     {
    //         printf("Build tool in development.\n");
    //         return 1;
    //     }

    //     // logic to run parser -> resolver -> installer whilst referencing events.jsonl from the user's project directory
    //     // ( after running bear intercept -- make )

    //     switch (tool)
    //     {
    //     case MAKE:
    //     {
    //         handle_make(argc, argv, tool);
    //         break;
    //     }

    //     case CMAKE:
    //     {
    //         handle_make(argc, argv, tool);
    //         break;
    //     }
    //     }
    //     break;
    // }

    // case CMD_REMOVE:
    // {
        
//         break;
//     }

//     case CMD_SHOW:
//     {
//         if (show_environments() < 0)
//             return 1;
//         break;
//     }

//     case CMD_SHOW_ENV:
//     {
//         char env_name[256];

//         if (argc >= 3)
//         {
//             snprintf(env_name, sizeof(env_name), "%s", argv[2]);
//         }

//         show_env_packages(env_name);
//         break;
//     }

//     case CMD_UPDATE_DB:
//     {
       
//     }

//     case CMD_UNKNOWN:
//     {
//         printf("Command not recognized. Use `virt-pack help` to see valid commands.");
//         break;
//     }
//     }
// }
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
void remove_lib(){
    // char env_name[256]; // buffer to hold the directory path

    //     if (argc >= 3)
    //     {
    //         snprintf(env_name, sizeof(env_name), "%s", argv[2]);
    //     }
    //     else
    //     {
    //         printf("Usage: virt-pack remove <env-name>\n");
    //         printf("Use `virt-pack show` to show names of all environments.\n");
    //         return 1;
    //     }
    //     handle_remove(argc, argv);
}