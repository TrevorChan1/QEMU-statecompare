#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
# define MAX_NAME_LEN 16

enum state_type {
    TYPE_UINT8,
    TYPE_UINT16,
    TYPE_UINT32,
    TYPE_UINT64,
    TYPE_CHAR
};

typedef struct {
    char name[MAX_NAME_LEN];
    uint32_t size;
    uint32_t offset;
    uint32_t num_elements;
    uint32_t element_size;
    enum state_type datatype;

} metadata_field;

typedef struct {
    uint16_t num_fields;
    uint16_t metadata_offset;
} metadata_header;



int main() {
    // Open file pointer for metadata and data
    FILE * metadata_fp = fopen("trevtesttest", "w");
    FILE * data_fp = fopen("trevtesttest", "w");

    // Initialize metadata header values
    metadata_header test_header;
    test_header.num_fields = 2;
    test_header.metadata_offset = sizeof(metadata_header);

    // Write the metadata header to the file
    fwrite(&test_header, sizeof(metadata_header), 1, metadata_fp);
    int data_offset = sizeof(metadata_header) + test_header.num_fields * sizeof(metadata_field);
    fseek(data_fp, data_offset, SEEK_SET);

    // Initialize 1st metadata entry values
    metadata_field test;
    char * name = (char*) "test_name";
    strcpy(test.name, name);
    test.size = sizeof(char) * 10;
    test.num_elements = 10;
    test.element_size = sizeof(char);
    test.offset = data_offset;
    test.datatype = TYPE_CHAR;
    
    char * data = (char*) "test_data";
    // Write metadata entry to metadata
    fwrite(&test, sizeof(metadata_field), 1, metadata_fp);

    // Write data to data
    fwrite(data, test.element_size, test.num_elements, data_fp);
    data_offset += test.size * sizeof(char);
    fseek(data_fp, data_offset, SEEK_SET);


    // Initialize 2nd metadata entry value
    metadata_field test2;
    char * name2 = (char*) "int";
    strcpy(test2.name, name2);
    test2.size = sizeof(uint32_t) * 3;
    test2.num_elements = 3;
    test2.element_size = sizeof(uint32_t);
    test2.offset = data_offset;
    test2.datatype = TYPE_UINT32;
    
    uint32_t data2[3] = {(uint32_t) 1, (uint32_t) 3, (uint32_t) 4};
    // Write metadata entry to metadata
    fwrite(&test2, sizeof(metadata_field), 1, metadata_fp);

    // Write data to data
    fwrite(data2, test2.element_size, test2.num_elements, data_fp);

    fclose(metadata_fp);
    fclose(data_fp);
    return 0;

}