#ifdef LOCATION
#ifdef __cplusplus
extern "C" {
#endif

#include "liblwm2m.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef LWM2M_CLIENT_MODE

#define VELOCITY_OCTETS                      5  // for HORIZONTAL_VELOCITY_WITH_UNCERTAINTY

typedef struct _location_data_t_
{
    struct _location_data_t_ * next;
    double    latitude;
    double    longitude;
    double    altitude;
    double    radius;
    uint8_t  velocity   [VELOCITY_OCTETS];        //3GPP notation 1st step: HORIZONTAL_VELOCITY_WITH_UNCERTAINTY
    unsigned long timestamp;
    double    speed;
    uint16_t shortID;
} location_data_t;

static uint8_t prv_res2tlv(lwm2m_data_t* dataP, location_data_t* locDataP);

static uint8_t prv_location_read(uint16_t objInstId,  int*  numDataP, lwm2m_data_t** tlvArrayP,lwm2m_object_t*  objectP);
static uint8_t prv_location_write(uint16_t instanceId,                                  int numData,                                  lwm2m_data_t * dataArray,                                  lwm2m_object_t * objectP);
void location_setVelocity(location_data_t *locationObj,
                          uint16_t bearing,
                          uint16_t horizontalSpeed,
                          uint8_t speedUncertainty);

void location_setLocationAtTime(lwm2m_object_t* locationObj,  float latitude,  float longitude, float altitude,uint64_t timestamp);
lwm2m_object_t * get_object_location();

void free_object_location(lwm2m_object_t * object);

void display_location_object(lwm2m_object_t * object);

#endif  //LWM2M_CLIENT_MODE

#ifdef __cplusplus
}
#endif
#endif
