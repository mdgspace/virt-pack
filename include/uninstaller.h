#ifndef UNINSTALLER_H
#define UNINSTALLER_H

#include <jansson.h>

#define DATA_DIR "data/"
#define INSTALLED_FILE DATA_DIR "installed.json"

void apt_remove(const char *pkgname);
int remove_pkg();

#endif