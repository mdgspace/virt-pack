#include "../../include/commands.h"
#include "../../include/installer.h"
#include "../../include/parser.h"
#include "../../include/resolver.h"
#include "../../include/util.h"
#include "string.h"

char *join_args(int argc, char *argv[]); 
void bear_intercept(int argc, char *argv[])//lets try parsing the entire command instead
//check what does env_name do
{
    printf("Intercepting using bear...\n");

    // get local directory path
    char local_dir[PATH_MAX];
    get_local_dir(local_dir, sizeof(local_dir));
    char cmd[256];
    //prepend bear intercept to current command
    char* command=join_args(argc,argv);//! add malloc
    snprintf(cmd,sizeof(cmd),"bear intercept -- %s",command);//!256 BITS MIGHT TRUNCATE
    printf("Running: %s\n",cmd);
    int ret = system(cmd);
    if (ret != 0)
    {
        fprintf(stderr, "[ERROR] Failed to run bear intercept command\n");
        return;
    }
    //! aage ka should not be required

}
char *join_args(int argc, char *argv[]) {
    int total_len = 0;
    //! what if argc less than 1
    for (int i = 0; i < argc; i++) {
        total_len += strlen(argv[i]) + 1; // +1 for space or '\0'
    }

    char *result = malloc(total_len);
    if (!result) return NULL;

    result[0] = '\0';

    for (int i = 0; i < argc; i++) {
        strcat(result, argv[i]);
        if (i < argc - 1) strcat(result, " ");
    }

    return result;
}
void handle_make(int argc, char *argv[])
{
    // get env name
    //char env_name[256];
    //snprintf(env_name, sizeof(env_name), "%s", argv[2]);

    bear_intercept(argc, argv);
    parser_main();
    resolver_main();
    installer_main();
}