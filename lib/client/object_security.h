#ifdef __cplusplus
extern "C" {
#endif

#ifndef WAKAAMA_OBJECT_SECURITY_H
#define WAKAAMA_OBJECT_SECURITY_H

#include "liblwm2m.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct _security_instance_
{
    struct _security_instance_ * next;        // matches lwm2m_list_t::next
    uint16_t                     instanceId;  // matches lwm2m_list_t::id
    char *                       uri;
    bool                         isBootstrap;
    uint8_t                      securityMode;
    char *                       publicIdentity;
    uint16_t                     publicIdLen;
    char *                       serverPublicKey;
    uint16_t                     serverPublicKeyLen;
    char *                       secretKey;
    uint16_t                     secretKeyLen;
    uint8_t                      smsSecurityMode;
    char *                       smsParams; // SMS binding key parameters
    uint16_t                     smsParamsLen;
    char *                       smsSecret; // SMS binding secret key
    uint16_t                     smsSecretLen;
    uint16_t                     shortID;
    uint32_t                     clientHoldOffTime;
    uint32_t                     bootstrapServerAccountTimeout;
} security_instance_t;


static uint8_t prv_security_read(uint16_t instanceId, int * numDataP, lwm2m_data_t ** dataArrayP,lwm2m_object_t * objectP);

#ifdef LWM2M_BOOTSTRAP

static uint8_t prv_security_write(uint16_t instanceId,  int numData,  lwm2m_data_t * dataArray,lwm2m_object_t * objectP);

static uint8_t prv_security_delete(uint16_t id, lwm2m_object_t * objectP);

static uint8_t prv_security_create(uint16_t instanceId,   int numData,  lwm2m_data_t * dataArray,  lwm2m_object_t * objectP);

#endif

void copy_security_object(lwm2m_object_t * objectDest, lwm2m_object_t * objectSrc);

void clean_security_object(lwm2m_object_t * objectP);

lwm2m_object_t * get_security_object(int serverId,const char* serverUri,  char * bsPskId, char * psk, uint16_t pskLen,bool isBootstrap);

char * get_server_uri(lwm2m_object_t * objectP, uint16_t secObjInstID);

#endif //WAKAAMA_NBIOT_H

#ifdef __cplusplus
}
#endif
