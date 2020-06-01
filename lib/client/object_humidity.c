#ifdef HUMIDITY

/*
 MIT Licence
 David Graeff <david.graeff@web.de>
*/

/*
 * Implements an object for screen control
 *
 *                 Multiple
 * Object |  ID  | Instances | Mandatoty |
 *  Test  | 1024 |    Yes    |    No     |
 *
 *  Ressources:
 *              Supported    Multiple
 *  Name | ID | Operations | Instances | Mandatory |  Type   | Range | Units |      Description      |
 *  state|  0 |    R/W     |    No     |    Yes    |   Bool  |       |       |                       |
 *
 */

#include <Arduino.h> 
#include "object_humidity.h"

static uint8_t prv_write(uint16_t instanceId,
                         int numData,
                         lwm2m_data_t *dataArray,
                         lwm2m_object_t *objectP)
{
    humidity_instance_t *targetP;
    int i;

    targetP = (humidity_instance_t *)lwm2m_list_find(objectP->instanceList, instanceId);
    if (NULL == targetP)
        return COAP_404_NOT_FOUND;

    for (i = 0; i < numData; i++)
    {
        switch (dataArray[i].id)
        {

        case SENSOR_H_VALUE:
            if (1 != lwm2m_data_decode_float(dataArray + i, &(targetP->sensor_value)))
            {
                return COAP_400_BAD_REQUEST;
            }
            break;

        default:
            return COAP_404_NOT_FOUND;
        }
    }

    return COAP_204_CHANGED;
}

static uint8_t prv_humidity_read(uint16_t instanceId, int *numDataP, lwm2m_data_t **dataArrayP, lwm2m_object_t *objectP)
{
    humidity_instance_t *targetP;
    int i;

    targetP = (humidity_instance_t *)lwm2m_list_find(objectP->instanceList, instanceId);
    if (NULL == targetP)
        return COAP_404_NOT_FOUND;

    if (*numDataP == 0)
    {
        *dataArrayP = lwm2m_data_new(6);
        if (*dataArrayP == NULL)
            return COAP_500_INTERNAL_SERVER_ERROR;
        *numDataP = 6;
        (*dataArrayP)[0].id = MIN_H_MEASURED_VALUE;
        (*dataArrayP)[1].id = MAX_H_MEASURED_VALUE;
        (*dataArrayP)[2].id = MIN_H_RANGE_VALUE;
        (*dataArrayP)[3].id = MAX_H_RANGE_VALUE;
        (*dataArrayP)[4].id = SENSOR_H_VALUE;
        (*dataArrayP)[5].id = SENSOR_H_UNITS;
    }

    for (i = 0; i < *numDataP; i++)
    {
        switch ((*dataArrayP)[i].id)
        {
        case MIN_H_MEASURED_VALUE:
            lwm2m_data_encode_float((double)targetP->min_measured_value, *dataArrayP + i);
            break;
        case MAX_H_MEASURED_VALUE:
            lwm2m_data_encode_float((double)targetP->max_measured_value, *dataArrayP + i);
            break;
        case MIN_H_RANGE_VALUE:
            lwm2m_data_encode_float((double)targetP->min_range_value, *dataArrayP + i);
            break;
        case MAX_H_RANGE_VALUE:
            lwm2m_data_encode_float((double)targetP->max_range_value, *dataArrayP + i);
            break;
        case SENSOR_H_VALUE:
            lwm2m_data_encode_float((double)targetP->sensor_value, *dataArrayP + i);
            break;
        case SENSOR_H_UNITS:
            lwm2m_data_encode_string(targetP->units, *dataArrayP + i);
            break;
        default:
            return COAP_404_NOT_FOUND;
        }
    }

    return COAP_205_CONTENT;
}

static uint8_t prv_exec(uint16_t instanceId,
                        uint16_t resourceId,
                        uint8_t *buffer,
                        int length,
                        lwm2m_object_t *objectP)
{

    if (NULL == lwm2m_list_find(objectP->instanceList, instanceId))
        return COAP_404_NOT_FOUND;

    switch (resourceId)
    {
    case RESET_H_MEASURED_VALUE:
        return COAP_204_CHANGED;
    default:
        return COAP_404_NOT_FOUND;
    }
}

lwm2m_object_t *init_humidity_object()
{
    //    Serial.println("init_humidity_object"); TODO

    lwm2m_object_t *humidityObj;

    humidityObj = (lwm2m_object_t *)lwm2m_malloc(sizeof(lwm2m_object_t));

    if (NULL != humidityObj)
    {
        int i;
        humidity_instance_t *targetP;

        memset(humidityObj, 0, sizeof(lwm2m_object_t));

        humidityObj->objID = HUMI_OBJECT_ID; 
        for (i = 0; i < 1; i++)
        {
            targetP = (humidity_instance_t *)lwm2m_malloc(sizeof(humidity_instance_t));
            if (NULL == targetP)
                return NULL;
            memset(targetP, 0, sizeof(humidity_instance_t));
            targetP->shortID = LWM2M_HUMIDITY_SHORT_OBJECT_ID;
            targetP->min_measured_value = 0;
            targetP->max_measured_value = 0;
            targetP->min_range_value = 0;
            targetP->max_range_value = 0;
            targetP->sensor_value = 0;
            strcpy(targetP->units,"%");
            humidityObj->instanceList = LWM2M_LIST_ADD(humidityObj->instanceList, targetP);
        }
        /*
         * From a single instance object, two more functions are available.
         * - The first one (createFunc) create a new instance and filled it with the provided informations. If an ID is
         *   provided a check is done for verifying his disponibility, or a new one is generated.
         * - The other one (deleteFunc) delete an instance by removing it from the instance list (and freeing the memory
         *   allocated to it)
         */
        humidityObj->readFunc = prv_humidity_read;
        humidityObj->writeFunc = prv_write;
        humidityObj->executeFunc = prv_exec;
    }

    return humidityObj;
}

void free_object_humidity(lwm2m_object_t * object)
{
    lwm2m_list_free(object->instanceList);
    lwm2m_free(object);
}
#endif