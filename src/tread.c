#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bookcase.h"
#include "tr_ui.h"

int main(int argc, char *argv[])
{
    manifest_t mp;

    build_manifest(argv[1], &mp);

    print_manifest(&mp);

    display_ui(&mp);

	return 0;
}

