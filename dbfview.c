#include <stdio.h>
#include <fcntl.h>
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

    for (int i = 0;i < file->field_count; i++) {
        struct dbf_field* cur_field = file->fields[i];
        printf("%s %c %d %d\n", cur_field->name, cur_field->type, cur_field->length, cur_field->dec_cnt);
    }

    free(file->fields);
    free(file->records);
    free(file);

    return 0;
}