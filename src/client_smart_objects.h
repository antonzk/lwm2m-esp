#include "WiFiManager.h"
#include "LwM2mManager.h"
#ifdef WITH_PREFERENCE
#include "PreferenceManager.h"
#endif
#ifdef WITH_BLE
#include "BLEServerManager.h"
#endif


extern lwm2m_object_t *objArray[OBJ_COUNT];


void addSingleSmartObject(const char *smartObject, uint8_t countOfObjects, lwm2m_object_t *(*functInit)());

void addSmartObject(uint8_t* countOfObjects);
void updateSensors(lwm2m_context_t *context);

void updateBitmap(lwm2m_context_t *context);
void updateTemperature(lwm2m_context_t *context);
void updateGyrometer(lwm2m_context_t *context);
void updateLuminance(lwm2m_context_t *context);
void updateHumidity(lwm2m_context_t *context);
void updateAnalogOutput(lwm2m_context_t *context);
void updatePushButton();
void updateSignalStrength();
void updateAccelerometer();
void updatePIR();
void updateMagnetometer();
void updatePower();
void updateLoudness();
void updateLocation();


