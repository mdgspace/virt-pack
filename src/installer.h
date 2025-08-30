#ifndef INSTALLER_H
#define INSTALLER_H

#define DATA_DIR "data/"
#define MISSING_FILE DATA_DIR "missing-libs.json"
#define DB_FILE DATA_DIR "virt-pack-db.json"

void install_package(const char *pkgname);
int installer_main();

#endif
