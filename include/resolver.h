#ifndef RESOLVER_H
#define RESOLVER_H

#include <jansson.h>

int is_installed(const char *lib_name);
int resolver_main();

#endif