#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

#include "dbfviewlib.h"

struct dbf_file* file;

int main(int argc, char** argv) {
    if (argc > 2) {
        printf("Too much arguments passed.\n");

        return 1;
    }
    else if (argc < 2) {
        printf("No arguments passed.\n");

        return 1;
    }

    file = read_dbf(argv[1]);

    if (file == NULL) {
        return 1;
    }

    printf("%d\n", file->metadata.header_size);
    printf("%d\n", file->metadata.record_count);
    printf("%d\n", file->metadata.record_size);

    free(file->fields);
    free(file->records);
    free(file);

    return 0;
}