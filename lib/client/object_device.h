
#ifdef __cplusplus
extern "C" {
#endif

#ifndef WAKAAMA_OBJECT_DEVICE_H
#define WAKAAMA_OBJECT_DEVICE_H

#include "liblwm2m.h"
#include "lwm2mclient.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define PRV_OFFSET_MAXLEN   7 //+HH:MM\0 at max


typedef struct
{
    int64_t free_memory;
    int64_t error;
    int64_t time;
    int64_t battery_level;
    char time_offset[PRV_OFFSET_MAXLEN];
} device_data_t;



static int prv_check_time_offset(char * buffer,    int length);
static uint8_t prv_set_value(lwm2m_data_t * dataP,  device_data_t * devDataP);
static uint8_t prv_device_read(uint16_t instanceId,      int * numDataP,  lwm2m_data_t ** dataArrayP,   lwm2m_object_t * objectP);
static uint8_t prv_device_discover(uint16_t instanceId,     int * numDataP,  lwm2m_data_t ** dataArrayP, lwm2m_object_t * objectP);
static uint8_t prv_device_write(uint16_t instanceId,   int numData,   lwm2m_data_t * dataArray,  lwm2m_object_t * objectP);
static uint8_t prv_device_execute(uint16_t instanceId,   uint16_t resourceId,  uint8_t * buffer, int length, lwm2m_object_t * objectP);
lwm2m_object_t * get_object_device();
void free_object_device(lwm2m_object_t * objectP);
uint8_t device_change(lwm2m_data_t * dataArray,lwm2m_object_t * objectP);


#endif //WAKAAMA_NBIOT_H

#ifdef __cplusplus
}
#endif
