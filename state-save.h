#include <stdlib.h>
#include <stdio.h>
#include "qemu/osdep.h"
#include "hw/pci/pci_device.h"
#include "hw/scsi/esp.h"
#include "hw/usb/ccid.h"
#include "chardev/char-fe.h"
#include "net/net.h"
#include "hw/net/mii.h"

#define MAX_NAME_LEN 16

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

int vmstate_save_field(FILE * metadata_fp, FILE * data_fp, int offset, 
                        int element_size, int n, char * name, void * values);
int vmstate_save_pcidevice(FILE * metadata_fp, FILE * data_fp, int offset, PCIDevice * pci);
int vmstate_save_memoryregion(FILE * metadata_fp, FILE * data_fp, int offset, MemoryRegion * mr);
int vmstate_save_uint32(FILE * metadata_fp, FILE * data_fp, int offset, uint32_t * values, int n);
int vmstate_save_uint16(FILE * metadata_fp, FILE * data_fp, int offset, uint16_t * values, int n);
int vmstate_save_uint8(FILE * metadata_fp, FILE * data_fp, int offset, uint8_t * values, int n);
int vmstate_save_ESPState(FILE * metadata_fp, FILE * data_fp, int offset, ESPState * esp);
int vmstate_save_ccidstate(FILE * metadata_fp, FILE * data_fp, int offset, CCIDCardState * base);
int vmstate_save_charbackend(FILE * metadata_fp, FILE * data_fp, int offset, CharBackend * cb);
int vmstate_save_bool(FILE * metadata_fp, FILE * data_fp, int offset, bool * value, int n);
int vmstate_save_nicconf(FILE * metadata_fp, FILE * data_fp, int offset, NICConf * nic);
int vmstate_save_nicstate(FILE * metadata_fp, FILE * data_fp, int offset, NICState * ns);
