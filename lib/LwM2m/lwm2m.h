
#ifdef WITH_TINYDTLS
#include "dtlsconnection.h"
#else
#include "connection.h"
#endif

#include "lwm2mclient.h"

#include <string.h>


#define BACKUP_OBJECT_COUNT 2
#define OBJ_COUNT 9


#define MAX_PACKET_SIZE 1024
#define DEFAULT_SERVER_IPV6 "[::1]"
#define DEFAULT_SERVER_IPV4 "127.0.0.1"

typedef struct
{
    lwm2m_object_t * securityObjP;
    lwm2m_object_t * serverObject;
    int sock;
#ifdef WITH_TINYDTLS
    dtls_connection_t * connList;
    lwm2m_context_t * lwm2mH;
#else
    connection_t * connList;
#endif
    int addressFamily;
} client_data_t;

void prv_initiate_bootstrap(char *buffer, void *user_data);
void prv_backup_objects(lwm2m_context_t *context);
void prv_restore_objects(lwm2m_context_t *context);
void update_bootstrap_info(lwm2m_client_state_t *previousBootstrapState, lwm2m_context_t *context);
void close_backup_object();
void prv_remove(char *buffer, void *user_data);
void prv_add(char *buffer, void *user_data);
void prv_value_change(void *context, const char *uriPath, const char *value, size_t valueLength);
void init_value_change(lwm2m_context_t *lwm2m);
void system_reboot();
void *lwm2m_connect_server(uint16_t secObjInstID, void *userData);
void lwm2m_close_connection(void *sessionH, void *userData);
void handle_value_changed(lwm2m_context_t *lwm2mH, lwm2m_uri_t *uri, const char *value, size_t valueLength);
void handle_sigint(int signum);

float handle_value_read(lwm2m_context_t *lwm2mH, const char *uriText);
void update_event_value(lwm2m_context_t *context, const char *sensorUri, const char *stringValue);
void update_sensor_value(lwm2m_context_t *context, const char *sensorUri, double sensorValue);
void update_sensor_value_int(lwm2m_context_t *context, const char *sensorUri, int sensorValue);