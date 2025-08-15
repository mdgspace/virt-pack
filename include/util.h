#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "commands.h"

#define DATA_DIR "data/"

void get_local_dir(char *out, size_t size);
BuildTool detect_build_command(int argc, char *argv[]);

#endif