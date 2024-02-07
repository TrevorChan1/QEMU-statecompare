#include "state-save.h"

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

int vmstate_save_pcidevice(FILE * metadata_fp, FILE * data_fp, int offset, PCIDevice * pci) {
    // Initialize metadata entry
    metadata_field md;
    md.size = sizeof(PCIDevice);
    md.num_elements = 1;
    md.element_size = sizeof(PCIDevice);
    md.offset = offset;
    
    // Write metadata entry to metadata
    fwrite(&md, sizeof(metadata_field), 1, metadata_fp);

    // Write data to data
    fwrite(pci, md.element_size, md.num_elements, data_fp);

    // Return the offset to be used for the next entry
    int data_offset = offset += md.num_elements * md.element_size;
    return data_offset;

}

int vmstate_save_memoryregion(FILE * metadata_fp, FILE * data_fp, int offset, MemoryRegion * mr) {
    // Initialize metadata entry
    metadata_field md;
    md.size = mr->size;
    md.num_elements = 1;
    md.element_size = mr->size;
    md.offset = offset;
    
    // Write metadata entry to metadata
    fwrite(&md, sizeof(metadata_field), 1, metadata_fp);

    // Write data to data
    fwrite(mr, md.element_size, md.num_elements, data_fp);

    // Return the offset to be used for the next entry
    int data_offset = offset += md.num_elements * md.element_size;
    return data_offset;
}

int vmstate_save_uint32(FILE * metadata_fp, FILE * data_fp, int offset, uint32_t * values, int n) {
    // Initialize metadata entry
    metadata_field md;
    md.size = sizeof(uint32_t) * n;
    md.num_elements = n;
    md.element_size = sizeof(uint32_t);
    md.offset = offset;
    
    // Write metadata entry to metadata
    fwrite(&md, sizeof(metadata_field), 1, metadata_fp);

    // Write data to data
    fwrite(values, md.element_size, md.num_elements, data_fp);

    // Return the offset to be used for the next entry
    int data_offset = offset += md.num_elements * md.element_size;
    return data_offset;
}

int vmstate_save_uint16(FILE * metadata_fp, FILE * data_fp, int offset, uint16_t * values, int n) {
    // Initialize metadata entry
    metadata_field md;
    md.size = sizeof(uint16_t) * n;
    md.num_elements = n;
    md.element_size = sizeof(uint16_t);
    md.offset = offset;
    
    // Write metadata entry to metadata
    fwrite(&md, sizeof(metadata_field), 1, metadata_fp);

    // Write data to data
    fwrite(values, md.element_size, md.num_elements, data_fp);

    // Return the offset to be used for the next entry
    int data_offset = offset += md.num_elements * md.element_size;
    return data_offset;
}

int vmstate_save_uint8(FILE * metadata_fp, FILE * data_fp, int offset, uint8_t * values, int n) {
    // Initialize metadata entry
    metadata_field md;
    md.size = sizeof(uint8_t) * n;
    md.num_elements = n;
    md.element_size = sizeof(uint8_t);
    md.offset = offset;
    
    // Write metadata entry to metadata
    fwrite(&md, sizeof(metadata_field), 1, metadata_fp);

    // Write data to data
    fwrite(values, md.element_size, md.num_elements, data_fp);

    // Return the offset to be used for the next entry
    int data_offset = offset += md.num_elements * md.element_size;
    return data_offset;
}

int vmstate_save_ESPState(FILE * metadata_fp, FILE * data_fp, int offset, ESPState * esp) {
    // Initialize metadata entry
    metadata_field md;
    md.size = sizeof(ESPState);
    md.num_elements = 1;
    md.element_size = sizeof(ESPState);
    md.offset = offset;
    
    // Write metadata entry to metadata
    fwrite(&md, sizeof(metadata_field), 1, metadata_fp);

    // Write data to data
    fwrite(esp, md.element_size, md.num_elements, data_fp);

    // Return the offset to be used for the next entry
    int data_offset = offset += md.num_elements * md.element_size;
    return data_offset;
}

int vmstate_save_ccidstate(FILE * metadata_fp, FILE * data_fp, int offset, CCIDCardState * base) {
    // Initialize metadata entry
    metadata_field md;
    md.size = sizeof(CCIDCardState);
    md.num_elements = 1;
    md.element_size = sizeof(CCIDCardState);
    md.offset = offset;
    
    // Write metadata entry to metadata
    fwrite(&md, sizeof(metadata_field), 1, metadata_fp);

    // Write data to data
    fwrite(base, md.element_size, md.num_elements, data_fp);

    // Return the offset to be used for the next entry
    int data_offset = offset += md.num_elements * md.element_size;
    return data_offset;
}

int vmstate_save_charbackend(FILE * metadata_fp, FILE * data_fp, int offset, CharBackend * cb) {
    // Initialize metadata entry
    metadata_field md;
    md.size = sizeof(CharBackend);
    md.num_elements = 1;
    md.element_size = sizeof(CharBackend);
    md.offset = offset;
    
    // Write metadata entry to metadata
    fwrite(&md, sizeof(metadata_field), 1, metadata_fp);

    // Write data to data
    fwrite(cb, md.element_size, md.num_elements, data_fp);

    // Return the offset to be used for the next entry
    int data_offset = offset += md.num_elements * md.element_size;
    return data_offset;
}

int vmstate_save_bool(FILE * metadata_fp, FILE * data_fp, int offset, bool * values, int n) {
    // Initialize metadata entry
    metadata_field md;
    md.size = sizeof(bool);
    md.num_elements = n;
    md.element_size = sizeof(bool);
    md.offset = offset;
    
    // Write metadata entry to metadata
    fwrite(&md, sizeof(metadata_field), 1, metadata_fp);

    // Write data to data
    fwrite(values, md.element_size, md.num_elements, data_fp);

    // Return the offset to be used for the next entry
    int data_offset = offset += md.num_elements * md.element_size;
    return data_offset;
}

int vmstate_save_nicconf(FILE * metadata_fp, FILE * data_fp, int offset, NICConf * nic) {
    // Initialize metadata entry
    metadata_field md;
    md.size = sizeof(NICConf);
    md.num_elements = 1;
    md.element_size = sizeof(NICConf);
    md.offset = offset;
    
    // Write metadata entry to metadata
    fwrite(&md, sizeof(metadata_field), 1, metadata_fp);

    // Write data to data
    fwrite(nic, md.element_size, md.num_elements, data_fp);

    // Return the offset to be used for the next entry
    int data_offset = offset += md.num_elements * md.element_size;
    return data_offset;
}

int vmstate_save_nicstate(FILE * metadata_fp, FILE * data_fp, int offset, NICState * ns) {
    // Initialize metadata entry
    metadata_field md;
    md.size = sizeof(NICState);
    md.num_elements = 1;
    md.element_size = sizeof(NICState);
    md.offset = offset;
    
    // Write metadata entry to metadata
    fwrite(&md, sizeof(metadata_field), 1, metadata_fp);

    // Write data to data
    fwrite(ns, md.element_size, md.num_elements, data_fp);

    // Return the offset to be used for the next entry
    int data_offset = offset += md.num_elements * md.element_size;
    return data_offset;
}

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