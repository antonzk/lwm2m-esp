#ifdef __cplusplus
extern "C" {
#endif

#ifndef WAKAAMA_OBJECT_ACCESS_CONTROL_H
#define WAKAAMA_OBJECT_ACCESS_CONTROL_H

#include "liblwm2m.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct acc_ctrl_ri_s
{   // linked list:
    struct acc_ctrl_ri_s*   next;       // matches lwm2m_list_t::next
    uint16_t                resInstId;  // matches lwm2m_list_t::id, ..serverID
    // resource data:
    uint16_t                accCtrlValue;
} acc_ctrl_ri_t;

typedef struct acc_ctrl_oi_s
{   //linked list:
    struct acc_ctrl_oi_s*   next;       // matches lwm2m_list_t::next
    uint16_t                objInstId;  // matches lwm2m_list_t::id
    // resources
    uint16_t                objectId;
    uint16_t                objectInstId;
    uint16_t                accCtrlOwner;
    acc_ctrl_ri_t*          accCtrlValList;
} acc_ctrl_oi_t;



static uint8_t prv_read(uint16_t instanceId, int * numDataP,  lwm2m_data_t** dataArrayP, lwm2m_object_t * objectP);

static bool prv_add_ac_val(acc_ctrl_oi_t* accCtrlOiP, uint16_t acResId, uint16_t acValue);

static uint8_t prv_write_resources(uint16_t instanceId, int numData,lwm2m_data_t* tlvArray, lwm2m_object_t* objectP, bool doCreate);

static uint8_t prv_write(uint16_t instanceId, int numData, lwm2m_data_t* tlvArray, lwm2m_object_t* objectP);

static uint8_t prv_delete(uint16_t id, lwm2m_object_t * objectP);

static uint8_t prv_create(uint16_t objInstId, int numData, lwm2m_data_t * tlvArray, lwm2m_object_t * objectP);

lwm2m_object_t * acc_ctrl_create_object(void);

void acl_ctrl_free_object(lwm2m_object_t * objectP);

bool  acc_ctrl_obj_add_inst (lwm2m_object_t* accCtrlObjP, uint16_t instId, uint16_t acObjectId, uint16_t acObjInstId, uint16_t acOwner);

bool acc_ctrl_oi_add_ac_val (lwm2m_object_t* accCtrlObjP, uint16_t instId,uint16_t acResId, uint16_t acValue);

#endif //WAKAAMA_NBIOT_H

#ifdef __cplusplus
}
#endif
