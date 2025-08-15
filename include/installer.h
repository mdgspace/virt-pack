#ifndef INSTALLER_H
#define INSTALLER_H

#include <jansson.h>

#define DATA_DIR "data/"
#define MISSING_FILE DATA_DIR "missing-libs.json"
#define DB_FILE DATA_DIR "virt-pack-db.json"

void install_package(const char *pkgname);
void record_installed_packages(json_t *installed_map, const char *env_name, const char *pkgname);
int installer_main(const char *env_name);

#endif