#ifdef __cplusplus
extern "C" {
#endif

#ifndef WAKAAMA_OBJECT_FIRMWARE_H
#define WAKAAMA_OBJECT_FIRMWARE_H

#include "liblwm2m.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct
{
    uint8_t state;
    bool supported;
    uint8_t result;
} firmware_data_t;


static uint8_t prv_firmware_read(uint16_t instanceId,int * numDataP, lwm2m_data_t ** dataArrayP,lwm2m_object_t * objectP);

static uint8_t prv_firmware_write(uint16_t instanceId,  int numData, lwm2m_data_t * dataArray, lwm2m_object_t * objectP);

static uint8_t prv_firmware_execute(uint16_t instanceId, uint16_t resourceId, uint8_t * buffer,  int length,lwm2m_object_t * objectP);

lwm2m_object_t * get_object_firmware(void);

void free_object_firmware(lwm2m_object_t * objectP);

#endif //WAKAAMA_NBIOT_H

#ifdef __cplusplus
}
#endif
