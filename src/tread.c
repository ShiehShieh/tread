#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bookcase.h"

int main(int argc, char *argv[])
{
    manifest_t test;

    build_manifest(argv[1], &test);

    print_manifest(&test);

	return 0;
}

