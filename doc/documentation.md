# Structure
The program is split in 2 parts, main program which parses arguments and handles logic, and package manager specific shell scripts, mostly written in posix shell or bash. To add support for a package manager, add the command for identifying the package manager, and add scripts to install, uninstall and query the package, you can refer existing scripts.
