#ifdef ANALOG_OUTPUT
#include "object_analog_output.h"

static uint8_t prv_write(uint16_t instanceId,
                         int numData,
                         lwm2m_data_t *dataArray,
                         lwm2m_object_t *objectP)
{
    analog_output_instance_t *targetP;
    targetP = (analog_output_instance_t *)lwm2m_list_find(objectP->instanceList, instanceId);
    if (NULL == targetP)
        return COAP_404_NOT_FOUND;
    for (int i = 0; i < numData; i++)
    {
        switch (dataArray[i].id)
        {
        case ANALOG_OUTPUT_CURRENT_VALUE:
            if (1 != lwm2m_data_decode_float(dataArray + i, &(targetP->analog_current_value)))
            {
                return COAP_400_BAD_REQUEST;
            }
            break;
        case ANALOG_OUTPUT_APPLICATION_TYPE:
            strncpy((targetP)->application_type, (char *)dataArray[i].value.asBuffer.buffer,
                    dataArray[i].value.asBuffer.length);
            (targetP)->application_type[dataArray[i].value.asBuffer.length] = '\0';
            break;
        default:
            return COAP_404_NOT_FOUND;
        }
    }

    return COAP_204_CHANGED;
}

static uint8_t prv_read(uint16_t instanceId,
                        int *numDataP,
                        lwm2m_data_t **dataArrayP,
                        lwm2m_object_t *objectP)
{

    analog_output_instance_t *targetP;
    int i;

    targetP = (analog_output_instance_t *)lwm2m_list_find(objectP->instanceList, instanceId);
    if (NULL == targetP)
        return COAP_404_NOT_FOUND;

    if (*numDataP == 0)
    {
        *dataArrayP = lwm2m_data_new(4);
        if (*dataArrayP == NULL)
            return COAP_500_INTERNAL_SERVER_ERROR;
        *numDataP = 4;
        (*dataArrayP)[0].id = ANALOG_OUTPUT_CURRENT_VALUE;
        (*dataArrayP)[1].id = ANALOG_OUTPUT_MAX_RANGE_VALUE;
        (*dataArrayP)[2].id = ANALOG_OUTPUT_MIN_RANGE_VALUE;
        (*dataArrayP)[3].id = ANALOG_OUTPUT_APPLICATION_TYPE;
    }

    for (i = 0; i < *numDataP; i++)
    {
        switch ((*dataArrayP)[i].id)
        {
        case ANALOG_OUTPUT_CURRENT_VALUE:
            lwm2m_data_encode_float((double)targetP->analog_current_value, *dataArrayP + i);
            break;
        case ANALOG_OUTPUT_MAX_RANGE_VALUE:
            lwm2m_data_encode_float((double)targetP->analog_max_value, *dataArrayP + i);
            break;
        case ANALOG_OUTPUT_MIN_RANGE_VALUE:
            lwm2m_data_encode_float((double)targetP->analog_min_value, *dataArrayP + i);
            break;
        case ANALOG_OUTPUT_APPLICATION_TYPE:
             lwm2m_data_encode_string(targetP->application_type, *dataArrayP + i);
            break;
        default:
            return COAP_404_NOT_FOUND;
        }
    }

    return COAP_205_CONTENT;
}

lwm2m_object_t *init_analog_output_object()
{
    lwm2m_object_t *analogOutputObj;

    analogOutputObj = (lwm2m_object_t *)lwm2m_malloc(sizeof(lwm2m_object_t));

    if (NULL != analogOutputObj)
    {
        int i;
        analog_output_instance_t *targetP;

        memset(analogOutputObj, 0, sizeof(lwm2m_object_t));

        analogOutputObj->objID = ANALOG_OUTPUT_ID;
        for (i = 0; i < 1; i++)
        {
            targetP = (analog_output_instance_t *)lwm2m_malloc(sizeof(analog_output_instance_t));
            if (NULL == targetP)
                return NULL;
            memset(targetP, 0, sizeof(analog_output_instance_t));
            targetP->shortID = LWM2M_ANALOG_OUTPUT_SHORT_OBJECT_ID;
            targetP->analog_current_value = 0;
            targetP->analog_max_value = 0;
            targetP->analog_min_value = 0;
            strcpy(targetP->application_type, "analog_type");
            analogOutputObj->instanceList = LWM2M_LIST_ADD(analogOutputObj->instanceList, targetP);
        }
        /*
         * From a single instance object, two more functions are available.
         * - The first one (createFunc) create a new instance and filled it with the provided informations. If an ID is
         *   provided a check is done for verifying his disponibility, or a new one is generated.
         * - The other one (deleteFunc) delete an instance by removing it from the instance list (and freeing the memory
         *   allocated to it)
         */
        analogOutputObj->readFunc = prv_read;
        analogOutputObj->writeFunc = prv_write;
    }

    return analogOutputObj;
}

void free_object_analog_output(lwm2m_object_t *object)
{
    lwm2m_list_free(object->instanceList);
    lwm2m_free(object);
}
#endif