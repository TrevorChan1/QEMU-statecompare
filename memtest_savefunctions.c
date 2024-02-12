#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
# define MAX_NAME_LEN 16

typedef struct {
    char name[MAX_NAME_LEN];
    uint32_t size;
    uint32_t offset;
    uint32_t num_elements;
    uint32_t element_size;

} metadata_field;

typedef struct {
    uint16_t num_fields;
    uint16_t metadata_offset;
} metadata_header;

FILE ** vmstate_init_statefile(char * filename, int num_fields);

FILE ** vmstate_init_statefile(char * filename, int num_fields) {
    // Initialize file pointer output for metadata and data pointers
    FILE ** fp = malloc(2 * sizeof(FILE *));
    fp[0] = fopen(filename, "w");
    fp[1] = fopen(filename, "w");

    // Initialize metadata header values
    metadata_header header;
    header.num_fields = num_fields;
    header.metadata_offset = sizeof(metadata_header);

    // Write the metadata header to the file
    fwrite(&header, sizeof(metadata_header), 1, fp[0]);
    int data_offset = sizeof(metadata_header) + header.num_fields * sizeof(metadata_field);
    fseek(fp[1], data_offset, SEEK_SET);

    return fp;
}

int vmstate_save_field(FILE * metadata_fp, FILE * data_fp, int offset, 
                        int element_size, int n, char * name, void * values);
int vmstate_save_field(FILE * metadata_fp, FILE * data_fp, int offset, 
                        int element_size, int n, char * name, void * values) {
    // Initialize metadata entry
    metadata_field md;
    strncpy(md.name, name, MAX_NAME_LEN);
    md.size = element_size * n;
    md.num_elements = n;
    md.element_size = element_size;
    md.offset = offset;
    
    // Write metadata entry to metadata
    fwrite(&md, sizeof(metadata_field), 1, metadata_fp);

    // Write data to data
    fwrite(values, md.element_size, md.num_elements, data_fp);

    // Return the offset to be used for the next entry
    int data_offset = offset += md.num_elements * md.element_size;
    return data_offset;
}

int main() {
    FILE ** fp = vmstate_init_statefile((char*) "trevtesttest2", 2);
    int offset = sizeof(metadata_header) + 2 * sizeof(metadata_field);
    char data1[9] = "test_datb";
    uint32_t data2[3] = {1, 2, 3};
    // uint32_t data3[2] = {0, 1};

    offset = vmstate_save_field(fp[0], fp[1], offset, sizeof(char), 10, (char*) "data1", data1);
    offset = vmstate_save_field(fp[0], fp[1], offset, sizeof(uint32_t), 3, (char*) "data2", data2);
    // offset = vmstate_save_field(fp[0], fp[1], offset, sizeof(uint32_t), 2, (char*) "data3", data3);

    fclose(fp[0]);
    fclose(fp[1]);
    free(fp);


}