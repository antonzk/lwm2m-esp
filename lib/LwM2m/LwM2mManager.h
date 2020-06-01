#include "lwm2mclient.h"
#include "lwm2m.h"


#define OBJ_COUNT 15

class LwM2mManager
{
public:
bool init();
void lifecycle();
void clear();
private:

  client_data_t data;
  int result;
  lwm2m_context_t *lwm2mH = NULL;
  int lifetime = 40;
  //TODO - move to platfomrio.ini
  char *name = "DEVICE_NAME";
  lwm2m_object_t *objArray[OBJ_COUNT];
  const char *localPort = "56830";
  //TODO - move to platfomrio.ini
  const char *server = "127.0.0.1";
  // const char *server = NULL;
  // const char *serverPort = LWM2M_STANDARD_PORT_STR;
  //TODO - move to platfomrio.ini
  const char *serverPort = "5683";
  bool bootstrapRequested = false;
#ifdef LWM2M_BOOTSTRAP
  lwm2m_client_state_t previousState = STATE_INITIAL;
#endif

  char *pskBuffer = NULL;
  char *pskId = NULL;
  char *psk = NULL;
  uint16_t pskLen = -1;

  //////// Custom variables ////////////

  // Counter of objects for LwM2m context
  uint8_t objectCount = 0;
};

extern LwM2mManager LwM2m;