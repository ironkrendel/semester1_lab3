struct dbf_field {
    char* name;
    char type;
    unsigned char length;
    unsigned char dec_cnt;
    unsigned int id;
    unsigned char example;
    bool mdx_flag;
};

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
    void** fields;
    int field_count;
    unsigned char*** records;
};