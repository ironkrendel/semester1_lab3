#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#include "dbfviewlib.h"

#ifndef max
    #define max(a,b) ((a) > (b) ? (a) : (b))
#endif

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

    printf("%d\n", file->metadata.record_count);
    printf("%d\n", file->metadata.record_size);
    for (int i = 0;i < file->field_count; i++) {
        struct dbf_field* cur_field = file->fields[i];
        if (file->metadata.language_id == 0) {
            printf("%-20s ", cur_field->name);
        }
        else {
            printf("%-*s ", 20 - strlen(cur_field->name) / 2 + strlen(cur_field->name), cur_field->name);
        }
    }
    printf("\n");

    for (int i = 0;i < file->metadata.record_count;i++) {
        for (int j = 0;j < file->field_count;j++) {
            struct dbf_field* cur_field = file->fields[j];
            char* buf = malloc((cur_field->length + 1));
            strncpy(buf, &(file->records[i][j]), cur_field->length);
            buf[cur_field->length] = '\0';
            
            if (cur_field->type == 'C') {
                printf("%-*s ", max(cur_field->length, 20), buf);
                // for (int k = 0;k < cur_field->length;k++) {
                //     // putchar(((char*)file->records[i][j])[0]);
                // }
            }
            else if (cur_field->type == 'N') {
                printf("%-*d ", max(cur_field->length, 20), atoi(buf));
            }
            else if (cur_field->type == 'D') {
                printf("%-*s ", max(cur_field->length, 20), buf);
            }
            else {
                printf("%-*s ", max(cur_field->length, 20), buf);
            }

            free(buf);
        }
        printf("\n");
    }
    return 0;
}