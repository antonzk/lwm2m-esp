#ifdef __cplusplus
extern "C" {
#endif

#ifndef WAKAAMA_OBJECT_CONNECTIVITY_STAT_H
#define WAKAAMA_OBJECT_CONNECTIVITY_STAT_H

#include "liblwm2m.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct
{
    int64_t   smsTxCounter;
    int64_t   smsRxCounter;
    int64_t   txDataByte;             // report in kByte!
    int64_t   rxDataByte;             // report in kByte!
    int64_t   maxMessageSize;
    int64_t   avrMessageSize;
    int64_t   messageCount;           // private for incremental average calc.
    bool      collectDataStarted;
} conn_s_data_t;


static uint8_t prv_read(uint16_t instanceId, int * numDataP, lwm2m_data_t** dataArrayP, lwm2m_object_t * objectP);

static void prv_resetCounter(lwm2m_object_t* objectP, bool start);


void conn_s_updateTxStatistic(lwm2m_object_t * objectP, uint16_t txDataByte, bool smsBased);

void conn_s_updateRxStatistic(lwm2m_object_t * objectP, uint16_t rxDataByte, bool smsBased);

lwm2m_object_t * get_object_conn_s(void);

void free_object_conn_s(lwm2m_object_t * objectP);

#endif //WAKAAMA_NBIOT_H

#ifdef __cplusplus
}
#endif
