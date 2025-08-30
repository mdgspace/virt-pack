#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <getopt.h>

#include "commands.h"

#define PATH_MAX 4096

void print_version();
void update_db();
void remove_lib();

// ~/.local/share/virt-pack for persistent files
// ~/.cache/virt-pack for non-critical logs or intermediate files

void print_help()
{
    const char *help_text =
        "\n"
        "Usage:\n"
        "   virt-pack make <env-name>\n"
        "   virt-pack remove <env-name>\n"
        "   virt-pack show\n"
        "   virt-pack update-db\n"
        "   virt-pack --version | virt-pack version\n"
        "   virt-pack --help | virt-pack help\n";

    fputs(help_text, stdout);
}

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

