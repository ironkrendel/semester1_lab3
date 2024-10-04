#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include "dbfstructs.h"

#define read_param(p, s) ({ \
    err_code = read(file_index, p, s); \
    if (err_code < 0) { \
        printf("Encountered an error when reading file. Error number: %d\n", errno); \
        return NULL; \
    } \
    })

#define skip_bytes(n) ({ \
    void* dump = malloc(n); \
    read_param(dump, n); \
    free(dump); \
    })

void* read_dbf(char* filepath) {
    int file_index = open(filepath, O_RDONLY);
    struct dbf_file* file = malloc(sizeof(struct dbf_file));
    struct dbf_metadata* metadata = malloc(sizeof(struct dbf_metadata));

    if (file_index == -1) {
        printf("Error when opening file.\n");

        return NULL;
    }

    int err_code;

    read_param(&metadata->general_info, 1);
    read_param(&metadata->last_update, 3);
    read_param(&metadata->record_count, 4);
    read_param(&metadata->header_size, 2);
    read_param(&metadata->record_size, 2);
    skip_bytes(2);
    read_param(&metadata->incomplete_transaction, 1);
    read_param(&metadata->encryption_flag, 1);
    skip_bytes(12);
    read_param(&metadata->mdx_flag, 1);
    read_param(&metadata->language_id, 1);
    skip_bytes(2);

    int desc_cnt = 0;
    unsigned char* desc_buf;
    read_param(&desc_buf, 1);

    while (desc_buf != 13) {
        desc_cnt++;

        file->fields = realloc(file->fields, sizeof(void*) * desc_cnt);

        struct dbf_field* field = malloc(sizeof(struct dbf_field));

        char* name_buf = malloc(10);
        read_param(name_buf, 10);
        field->name = malloc(11);
        field->name[0] = desc_buf;
        strcpy(field->name + 1, name_buf);
        read_param(&field->type, 1);
        skip_bytes(4);
        read_param(&field->length, 1);
        read_param(&field->dec_cnt, 1);
        read_param(&field->id, 2);
        read_param(&field->example, 1);
        skip_bytes(10);
        read_param(&field->mdx_flag, 1);

        file->fields[desc_cnt - 1] = field;

        read_param(&desc_buf, 1);
    }

    file->metadata = *metadata;
    file->field_count = desc_cnt;
    file->records = NULL;

    close(file_index);

    return file;
}