#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdbool.h>

#define read_param(p, s) ({ \
    err_code = read(file_index, p, s); \
    if (err_code < 0) { \
        printf("Encountered an error when reading file. Error number: %d\n", err_code); \
        return NULL; \
    } \
    })

#define skip_bytes(n) ({ \
    void* dump = malloc(n); \
    read_param(dump, n); \
    free(dump); \
    })

struct dbf_metadata {
    unsigned char general_info;
    unsigned long last_update;
    unsigned long record_count;
    unsigned int header_size;
    unsigned int record_size;
    bool incomplete_transaction;
    bool encryption_flag;
    bool mdx_flag;
    unsigned char language_id;
};

struct dbf_file{
    struct dbf_metadata metadata;
    void* fields;
};

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

    file->fields = malloc(metadata->record_count * metadata->record_size);

    file->metadata = *metadata;
    file->fields = NULL;

    free(metadata);
    close(file_index);

    return file;
}