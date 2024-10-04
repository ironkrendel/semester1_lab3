#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdbool.h>

struct dbf_metadata {
    unsigned char general_info;
    unsigned long last_update;
    unsigned long record_count;
    unsigned int header_size;
    unsigned int record_size;
    bool incomplete_transaction;
    bool encryption_flag;
    bool mdx_exists;
    unsigned char language_id;
};

struct dbf_file{
    struct dbf_metadata metadata;
    void* fields;
};

void* read_dbf(char* filepath) {
    int file_index = open(filepath, O_RDONLY);

    if (file_index == -1) {
        printf("Error when opening file.\n");

        return NULL;
    }
}