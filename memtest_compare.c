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
    enum state_type datatype;
} metadata_field;

typedef struct {
    uint16_t num_fields;
    uint16_t metadata_offset;
} metadata_header;

int vmstate_compare(char * file1, char * file2);

int vmstate_compare(char* file1, char* file2) {
    // Open file pointer for metadata and data
    FILE * fd1 = fopen(file1, "r");
    FILE * fd2 = fopen(file2, "r");

    // Get metadata header
    metadata_header header1;
    metadata_header header2;
    fread(&header1, sizeof(metadata_header), 1, fd1);
    fread(&header2, sizeof(metadata_header), 1, fd2);

    // Check that the number of fields is the same
    if (header1.num_fields != header2.num_fields) {
        printf("Number of fields differ\n");
        return -1;
    }
    // Cast the binary data to metadata fields
    metadata_field md1[header1.num_fields];
    metadata_field md2[header2.num_fields];
    fread(&md1, sizeof(metadata_field), header1.num_fields, fd1);
    fread(&md2, sizeof(metadata_field), header2.num_fields, fd2);

    int differences = 0;

    // Iterate through each of the fields, compare each one
    for (int i = 0; i < header1.num_fields; i++){
        fseek(fd1, md1[i].offset, SEEK_SET);
        fseek(fd2, md2[i].offset, SEEK_SET);

        // Check if the data sizes match
        if (md1[i].size != md2[i].size) {
            differences++;
            printf("%s Size differs\n", md1[i].name);
        }
        else {
            // Allocate memory for both files' data
            void* buffer1 = malloc(md1[i].size);
            void* buffer2 = malloc(md2[i].size);

            // Read data from files
            fread(buffer1, md1[i].element_size, md1[i].num_elements, fd1);
            fread(buffer2, md2[i].element_size, md2[i].num_elements, fd2);

            // Check if the memory is different
            if (memcmp(buffer1, buffer2, md1[i].size) != 0) {
                differences++;
                printf("%s Data differs\n", md1[i].name);
            }

            // Free allocated memory
            free(buffer1);
            free(buffer2);
        }
    }

    printf("%d differences\n", differences);
    // Close the files
    fclose(fd1);
    fclose(fd2);
    return differences;
}

int main() {
    char * filename1 = (char *) "trevtesttest";
    char * filename2 = (char *) "trevtesttest2";
    int test = vmstate_compare(filename1, filename2);
    return test;
}