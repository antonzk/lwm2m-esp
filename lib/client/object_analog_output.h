#ifdef ANALOG_OUTPUT
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef WAKAAMA_ANALOG_OUTPUT_H
#define WAKAAMA_ANALOG_OUTPUT_H

#include "liblwm2m.h"
#include "lwm2mclient.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LWM2M_ANALOG_OUTPUT_SHORT_OBJECT_ID 0
#define ANALOG_OUTPUT_NAME "analog_output"
#define ANALOG_OUTPUT_CURRENT_VALUE 5650
#define ANALOG_OUTPUT_APPLICATION_TYPE 5750
#define ANALOG_OUTPUT_MIN_RANGE_VALUE 5603
#define ANALOG_OUTPUT_MAX_RANGE_VALUE 5604

    typedef struct _analog_output_prv_instance_
    {
        struct _analog_output_prv_instance_ *next; // matches lwm2m_list_t::next
        uint16_t shortID;                          // matches lwm2m_list_t::id
        float analog_current_value;
        char application_type[20];
        float analog_min_value;
        float analog_max_value;
    } analog_output_instance_t;

    lwm2m_object_t *init_analog_output_object();

    void free_object_analog_output(lwm2m_object_t *object);

#endif //WAKAAMA_ANALOG_OUTPUT_H

#ifdef __cplusplus
}
#endif
#endif