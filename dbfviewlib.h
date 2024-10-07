#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include "dbfstructs.h"

#define read_param(p, n) ({\
    fread(p, sizeof(char) * n, 1, file_ptr);\
    })

#define skip_bytes(n) ({\
    fseek(file_ptr, n, SEEK_CUR);\
    })

void* read_dbf(char* filepath) {
    FILE* file_ptr = fopen(filepath, "r");
    struct dbf_file* file = malloc(sizeof(struct dbf_file));
    struct dbf_metadata* metadata = malloc(sizeof(struct dbf_metadata));

    if (file_ptr == NULL) {
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
    unsigned char desc_buf;
    read_param(&desc_buf, 1);

    while (((unsigned long long)desc_buf % 256) != 13) {
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

    skip_bytes(1);

    file->records = malloc(sizeof(void*) * metadata->record_count);

    for (int i = 0;i < metadata->record_count;i++) {
        file->records[i] = malloc(sizeof(void*) * desc_cnt);

        for (int j = 0;j < desc_cnt;j++) {
            file->records[i][j] = malloc(((struct dbf_field*)file->fields[j])->length);
            read_param(file->records[i][j], ((struct dbf_field*)file->fields[j])->length);
        }
        skip_bytes(1);
    }

    file->metadata = *metadata;
    file->field_count = desc_cnt;

    fclose(file_ptr);

    return file;
}