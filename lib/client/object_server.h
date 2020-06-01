#ifdef __cplusplus
extern "C" {
#endif

#ifndef WAKAAMA_OBJECT_SERVER_H
#define WAKAAMA_OBJECT_SERVER_H

#include "liblwm2m.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _server_instance_
{
    struct _server_instance_ * next;   // matches lwm2m_list_t::next
    uint16_t    instanceId;            // matches lwm2m_list_t::id
    uint16_t    shortServerId;
    uint32_t    lifetime;
    uint32_t    defaultMinPeriod;
    uint32_t    defaultMaxPeriod;
    uint32_t    disableTimeout;
    bool        storing;
    char        binding[4];
} server_instance_t;

static uint8_t prv_server_read(uint16_t instanceId,  int * numDataP,  lwm2m_data_t ** dataArrayP, lwm2m_object_t * objectP);

static uint8_t prv_server_discover(uint16_t instanceId,int * numDataP, lwm2m_data_t ** dataArrayP, lwm2m_object_t * objectP);

static uint8_t prv_set_int_value(lwm2m_data_t * dataArray, uint32_t * data) ;

static uint8_t prv_server_write(uint16_t instanceId, int numData,  lwm2m_data_t * dataArray,  lwm2m_object_t * objectP);

static uint8_t prv_server_execute(uint16_t instanceId, uint16_t resourceId,  uint8_t * buffer,  int length,  lwm2m_object_t * objectP);

static uint8_t prv_server_delete(uint16_t id,  lwm2m_object_t * objectP);

static uint8_t prv_server_create(uint16_t instanceId, int numData,  lwm2m_data_t * dataArray, lwm2m_object_t * objectP);

void copy_server_object(lwm2m_object_t * objectDest, lwm2m_object_t * objectSrc);

lwm2m_object_t * get_server_object(int serverId, const char* binding,   int lifetime, bool storing);

void clean_server_object(lwm2m_object_t * object);

#endif //WAKAAMA_NBIOT_H

#ifdef __cplusplus
}
#endif
