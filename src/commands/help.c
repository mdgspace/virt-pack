#include <stdio.h>

void print_help()
{
    const char *help_text =
        "\n"
        "Usage:\n"
        "   virt-pack make <env-name>\n"
        "   virt-pack remove <env-name>\n"
        "   virt-pack show\n"
        "   virt-pack update-db\n"
        "   virt-pack --version | virt-pack version\n"
        "   virt-pack --help | virt-pack help\n";

    fputs(help_text, stdout);
}
