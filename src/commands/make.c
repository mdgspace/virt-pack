#include "../../include/commands.h"
#include "../../include/util.h"

void bear_intercept(const char *env_name,const char* command)//lets try parsing the entire command instead
//check what does env_name do
{
    printf("Intercepting using bear...\n");

    // get local directory path
    char local_dir[PATH_MAX];
    get_local_dir(local_dir, sizeof(local_dir));
    char cmd[256];
    //prepend bear intercept to current command
    snprintf(cmd,sizeof(cmd),"bear intercept -- %s",command);//256 BITS MIGHT TRUNCATE
    printf("Running: %s\n",cmd);
    int ret = system(cmd);
    if (ret != 0)
    {
        fprintf(stderr, "[ERROR] Failed to run bear intercept command\n");
        return;
    }

    // source and destination paths
    char src_path[PATH_MAX];
    snprintf(src_path, sizeof(src_path), "events.json");

    char dest_path[PATH_MAX];
    snprintf(dest_path, sizeof(dest_path), "%s/%s-events.json", local_dir, env_name);

    FILE *src = fopen(src_path, "r");
    if (!src)
    {
        perror("[ERROR] Could not open source events.json. Check perms and try again\n");
        return;
    }

    FILE *dest = fopen(dest_path, "w");
    if (!dest)
    {
        perror("[ERROR] Could not create destination events.json. Check perms and try again\n");
        fclose(src);
        return;
    }

    char buffer[8192];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0)
    {
        fwrite(buffer, 1, bytes, dest);
    }

    fclose(src);
    fclose(dest);

    printf("(*) Copied events.json to %s\n", dest_path);
    printf("(*) bear intercept ended\n");
}

void handle_make(int argc, char *argv[], const char* command)
{
    // get env name
    char env_name[256];
    snprintf(env_name, sizeof(env_name), "%s", argv[2]);

    bear_intercept(env_name, command);
    parser_main(env_name);
    resolver_main();
    installer_main(env_name);
}