#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
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
} metadata_field;

typedef struct {
    uint16_t num_fields;
    uint16_t metadata_offset;
} metadata_header;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("ERROR: No filename inputted\n");
        return -1;
    }

    // Open file pointer for metadata and data
    FILE * fp = fopen(argv[1], "r");

    // Get metadata header
    metadata_header header;
    fread(&header, sizeof(metadata_header), 1, fp);
    printf("Metadata Header:\n# Elements:%d Offset to metadata array:%d\n", header.num_fields, header.metadata_offset);

    // Iterate through the metadata fields
    metadata_field md[header.num_fields];
    fread(&md, sizeof(metadata_field), header.num_fields, fp);
    for (int i = 0; i < header.num_fields; i++){
        printf("Metadata Field %d:\n", i);
        printf("\tName: %s\n", md[i].name);
        printf("\tSize: %d\n", md[i].size);
        printf("\tOffset: %d\n", md[i].offset);
        printf("\t# Elements: %d\n", md[i].num_elements);
        printf("\tElement Size: %d\n", md[i].element_size);
    }

    // Check the data is correct
    fseek(fp, md[0].offset, SEEK_SET);
    char test_read[md[0].size];
    fread(test_read, md[0].element_size, md[0].num_elements, fp);
    printf("Data 1: %s\n", test_read);

    fseek(fp, md[1].offset, SEEK_SET);
    uint32_t test_read2[md[1].size];
    fread(test_read2, md[1].element_size, md[1].num_elements, fp);
    printf("Data 2: ");
    for (int k = 0; k < md[1].num_elements; k++) {
        printf("%u ", test_read2[k]);
    }
    printf("\n");

    fclose(fp);

    return 0;

}