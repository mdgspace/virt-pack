#ifndef UNINSTALLER_H
#define UNINSTALLER_H

#include <jansson.h>

#define DATA_DIR "data/"
#define INSTALLED_FILE DATA_DIR "installed.json"

void uninstall_package(const char *pkgname);
int uninstaller_main(const char *env_name);

#endif