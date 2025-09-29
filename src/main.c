#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "commands.h"

#define PATH_MAX 4096
#define VIRT_PACK_LOCAL_DIR_PATH ".local/share/virt-pack"

char path[256];

const char *package_manager = NULL;

static const char *pkgmgr_bins[2] = { "apt", "xbps-install" }; 
static const char *pkgmgr_names[2] = { "apt", "xbps" };

const char *get_pkgmgr() {
    const char *path = getenv("PATH");
    if (!path) return 0;

    char *p = strdup(path);
    char *dir = strtok(p, ":");
    while (dir) {
        char buf[1024];
        for (int i = 0; i < sizeof(pkgmgr_bins)/sizeof(pkgmgr_bins[0]); i++) {
            snprintf(buf, sizeof(buf), "%s/%s", dir, pkgmgr_bins[i]);
            if (access(buf, X_OK) == 0) {
                free(p);
                return pkgmgr_names[i];
            }
        }
        dir = strtok(NULL, ":");
    }
    free(p);
    return NULL;
}

void print_help()
{
    const char *help_text =
        "\n"
        "Usage:\n"
        "   virt-pack -- build_command\n"
        "   virt-pack --remove\n"
        "   virt-pack --show\n"
        "   virt-pack --show-pkgmgr\n"
        "   virt-pack --info\n"
        "   virt-pack --version\n"
        "   virt-pack --help\n";
    fputs(help_text, stderr);
}

void show_pkgmgr() {
    fputs(package_manager, stderr);
}

void print_version() {
    const char *version = "virt-pack version 1.0.0\n";
    fputs(version, stderr);
}



int main(int argc, char *argv[]) {   
    const char *home = getenv("HOME");
    if (!home) {
        fprintf(stderr, "[ERROR] HOME not set.\n");
        return 1;
    }
    package_manager = get_pkgmgr();

    static struct option longopts[]={
        {"help",no_argument,NULL,'h'},
        {"version",no_argument,NULL,'v'},
        {"show-pkgmgr",no_argument,NULL,'s'},
        {"remove",no_argument,NULL,'r'},
        {"info",no_argument,NULL,'i'},
        {0,0,0,0}
    };
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
    while((option=getopt_long(getopt_argc,argv,"hvsri",longopts,NULL))!=-1){
        switch (option){
            case 'h':
                print_help();
                break;
            case 'v':
                print_version();
                break;
            case 'r':
                handle_remove(package_manager);
                break;
            case 's':
            	show_pkgmgr();
            	break;
            case 'i':
            	info_package(package_manager);
            	break;
            default:
                print_help();
                break;
        }
    }
    if(sep_index!=-1&&sep_index+1<argc){
        printf("Forwarding args after -- ");
        handle_make(argc-sep_index-1,&argv[sep_index+1], package_manager);
    }
}
