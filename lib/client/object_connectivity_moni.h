#ifdef __cplusplus
extern "C" {
#endif

#ifndef WAKAAMA_OBJECT_CONNECTIVITY_MONI_H
#define WAKAAMA_OBJECT_CONNECTIVITY_MONI_H

#include "liblwm2m.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct
{
    char ipAddresses[2][16];        // limited to 2!
    char routerIpAddresses[2][16];  // limited to 2!
    long cellId;
    int signalStrength;
    int linkQuality;
    int linkUtilization;
} conn_m_data_t;


static uint8_t prv_read(uint16_t instanceId, int * numDataP,lwm2m_data_t ** dataArrayP,lwm2m_object_t * objectP);


lwm2m_object_t * get_object_conn_m(void);


void free_object_conn_m(lwm2m_object_t * objectP);


uint8_t connectivity_moni_change(lwm2m_data_t * dataArray, lwm2m_object_t * objectP);

#endif //WAKAAMA_NBIOT_H

#ifdef __cplusplus
}
#endif
