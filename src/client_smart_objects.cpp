#include "client_smart_objects.h"
WiFiManager Wi_Fi;
LwM2mManager LwM2m;
#ifdef WITH_PREFERENCE
PreferenceManager pref;
#endif
#ifdef WITH_BLE
BLEServerManager BLE;
#endif

#if defined(DEBUG_LOG) || defined(DEVELOPER_LOG)
char debugBuffer[70] = {0};
#endif
double sensorValue = 0;
lwm2m_object_t *objArray[OBJ_COUNT];
#ifdef GYROMETER
LSM303 accel;
#endif

#ifdef BITMAP
Adafruit_MCP23008 mcp1;
Adafruit_MCP23008 mcp2;
#endif

char oirBuf[18] = "";

#define SLASH "/"
#define STR(t) #t
#define OIR_X(t) SLASH STR(t)
#define OIR(o, i, r) \
    OIR_X(o)         \
    OIR_X(i)         \
    OIR_X(r)

#define ANALOG_OUTPUT_OIR OIR(ANALOG_OUTPUT_ID, LWM2M_ANALOG_OUTPUT_SHORT_OBJECT_ID, ANALOG_OUTPUT_CURRENT_VALUE)
#define LUMINANCE_OIR OIR(LUMINANCE_OBJECT_ID, LWM2M_LUMINANCE_SHORT_OBJECT_ID, SENSOR_LUM_VALUE)
#define GYROMETER_X_OIR OIR(GYROMETER_ID, LWM2M_GYROMETER_SHORT_OBJECT_ID, X_G_VALUE)
#define GYROMETER_Y_OIR OIR(GYROMETER_ID, LWM2M_GYROMETER_SHORT_OBJECT_ID, Y_G_VALUE)
#define GYROMETER_Z_OIR OIR(GYROMETER_ID, LWM2M_GYROMETER_SHORT_OBJECT_ID, Z_G_VALUE)
#define HUMIDITY_OIR OIR(HUMI_OBJECT_ID, LWM2M_HUMIDITY_SHORT_OBJECT_ID, SENSOR_H_VALUE)
#define TEMPERATURE_OIR OIR(TEMP_OBJECT_ID, LWM2M_TEMPERATURE_SHORT_OBJECT_ID, SENSOR_T_VALUE)
#define BITMAP_OIR OIR(BITMAP_OBJECT_ID, LWM2M_BITMAP_SHORT_OBJECT_ID, BITMAP_INPUT)

void addSingleSmartObject(const char *smartObject, uint8_t *countOfObjects, lwm2m_object_t *(*functInit)())
{
    *countOfObjects += 1;
    objArray[*countOfObjects] = functInit();
#if defined(DEBUG_LOG) || defined(DEVELOPER_LOG)
    fprintf(stderr, "Added object %s. Object number: %d\r\n", smartObject, *countOfObjects);
#endif
    if (NULL == objArray[*countOfObjects])
    {
        *countOfObjects -= 1;
#if defined(DEBUG_LOG) || defined(DEVELOPER_LOG)
        fprintf(stderr, "Failed to create %d object.\r\n", smartObject);
#endif
    }
}

void addSmartObject(uint8_t *countOfObjects)
{
#ifdef COMMUNICATION_CHARACTER
    addSingleSmartObject(COMM_CHAR_NAME, countOfObjects, init_communication_characteristics_object);
#endif
#ifdef BITMAP
    addSingleSmartObject(BITMAP_NAME, countOfObjects, init_bitmap_object);
#endif
#ifdef GYROMETER
    addSingleSmartObject(GYROMETER_NAME, countOfObjects, init_gyrometer_object);
#endif
#ifdef TEMPERATURE
    addSingleSmartObject(TEMPERATURE_NAME, countOfObjects, init_temperature_object);
#endif
#ifdef HUMIDITY
    addSingleSmartObject(HUMIDITY_NAME, countOfObjects, init_humidity_object);
#endif
#ifdef LUMINANCE
    addSingleSmartObject(LUMINANCE_NAME, countOfObjects, init_luminance_object);
#endif
#ifdef DIGITAL_INPUT
    addSingleSmartObject(DIGITAL_INPUT_NAME, countOfObjects, init_digital_input_object);
#endif
#ifdef ANALOG_OUTPUT
    addSingleSmartObject(ANALOG_OUTPUT_NAME, countOfObjects, init_analog_output_object);
#endif
    // addSingleSmartObject("accelerometer", countOfObjects, init_accelerometer_object);
    // addSingleSmartObject("buzzer", countOfObjects, init_buzzer_object);
    // addSingleSmartObject("led", countOfObjects, init_led_object);
    // addSingleSmartObject("switch", countOfObjects, init_switch_relay_object);
    // addSingleSmartObject("location", countOfObjects, get_object_location);
    // addSingleSmartObject("loudness", countOfObjects, init_loudness_object);
    // addSingleSmartObject("power", countOfObjects, init_power_object);
    // addSingleSmartObject("push button", countOfObjects, init_magnetometer_object);
    // addSingleSmartObject("pir", countOfObjects, init_presence_object);
}

//////////////////////////////////////////////////
///////// UPDATE LwM2M SMART OBJECTS VALUE ////////////////////////
////////////////////////////////////////////////

void updateSensors(lwm2m_context_t *context)
{

#if defined(DEBUG_LOG) || defined(DEVELOPER_LOG)
    fprintf(stderr, "Update sensors data\r\n");
#endif

#ifdef BITMAP
    updateBitmap(context);
#endif
#ifdef TEMPERATURE
    updateTemperature(context);
#endif
#ifdef HUMIDITY
    updateHumidity(context);
#endif
#ifdef LUMINANCE
    updateLuminance(context);
#endif
#ifdef GYROMETER
    updateGyrometer(context);
#endif
#ifdef DIGITAL_INPUT
    updateDigitalInput(context);
#endif
#ifdef LOCATION
    updateLocation();
#endif
#ifdef LOUDNESS
    updateLoudness();
#endif
#ifdef POWER
    updatePower();
#endif
#ifdef MAGNETOMETER
    updateMagnetometer();
#endif
#ifdef ACCELEROMETER
    updateAccelerometer();
#endif
#ifdef PIR
    updatePIR();
#endif
#ifdef PUSH_BUTTON
    updatePushButton();
#endif
#ifdef SIGNAL_STRENGTH
    updateSignalStrength();
#endif
#ifdef ANALOG_OUTPUT
    updateAnalogOutput(context);
#endif
}

#ifdef ANALOG_OUTPUT
// --- LUMINANCE ------------------------------------

void updateAnalogOutput(lwm2m_context_t *context)
{
    update_sensor_value(context, ANALOG_OUTPUT_OIR, sensorValue);
}

#endif

#ifdef LUMINANCE
// --- LUMINANCE ------------------------------------
void updateLuminance(lwm2m_context_t *context)
{
    sensorValue = analogRead(LIGHT_VALUE_PIN);
#if defined(DEBUG_LOG) || defined(DEVELOPER_LOG)
    DEBUG_STREAM.print("LUMINANCE: ");
    DEBUG_STREAM.println(sensorValue);
#endif
    update_sensor_value(context, LUMINANCE_OIR, sensorValue);
}
#endif

#ifdef GYROMETER
// --- GYROMETER ------------------------------------
void updateGyrometer(lwm2m_context_t *context)
{
    accel.read();
#if defined(DEBUG_LOG) || defined(DEVELOPER_LOG)
    DEBUG_STREAM.print("GYROMETER: ");
    sensorValue = accel.a.x;
    DEBUG_STREAM.print("X=");
    DEBUG_STREAM.print(sensorValue);
    sensorValue = accel.a.y;
    DEBUG_STREAM.print(" Y=");
    DEBUG_STREAM.print(sensorValue);
    sensorValue = accel.a.y;
    DEBUG_STREAM.print("Z=");
    DEBUG_STREAM.println(sensorValue);
#endif
    update_sensor_value(context, GYROMETER_X_OIR, accel.a.x);
    update_sensor_value(context, GYROMETER_Y_OIR, accel.a.y);
    update_sensor_value(context, GYROMETER_Z_OIR, accel.a.z);
}
#endif

#ifdef HUMIDITY
// --- HUMIDITY ------------------------------------
void updateHumidity(lwm2m_context_t *context)
{
    sensorValue = hts221.readHumidity();
    sensorValue = roundf(sensorValue * 100) / 100;
#if defined(DEBUG_LOG) || defined(DEVELOPER_LOG)
    DEBUG_STREAM.print(F("HUMIDITY: "));
    DEBUG_STREAM.println(sensorValue);
#endif
    update_sensor_value(context, HUMIDITY_OIR, sensorValue);
}
#endif

#ifdef TEMPERATURE
//--- TEMPERATURE ----------------------------------------
void updateTemperature(lwm2m_context_t *context)
{
    sensorValue = hts221.readTemperature();
    sensorValue = roundf(sensorValue * 100) / 100;
#if defined(DEBUG_LOG) || defined(DEVELOPER_LOG)
    DEBUG_STREAM.print(F("TEMPERATURE: "));
    DEBUG_STREAM.println(sensorValue);
#endif
    update_sensor_value(context, TEMPERATURE_OIR, sensorValue);
}
#endif

#ifdef BITMAP
//--- BITMAP ----------------------------------------
void updateBitmap(lwm2m_context_t *context)
{
    update_sensor_value_int(context, BITMAP_OIR, mcp2.readGPIO());
#if defined(DEBUG_LOG) || defined(DEVELOPER_LOG)
    DEBUG_STREAM.print("BITMAP: ");
    DEBUG_STREAM.println(mcp2.readGPIO());
#endif
}
#endif

#ifdef DIGITAL_INPUT
//--- DIGITAL_INPUT ----------------------------------------
void updateDigitalInput(lwm2m_context_t *context)
{
    for (int i = 0; i < LWM2M_DIGITAL_INPUT_SHORT_OBJECT_ID; i++)
    {
        memset(oirBuf, 0, sizeof oirBuf);
#if defined(DEBUG_LOG) || defined(DEVELOPER_LOG)
        DEBUG_STREAM.print(F("DIGITAL_INPUT instance: "));
        DEBUG_STREAM.print(i);
        DEBUG_STREAM.print(F(" value: "));
        DEBUG_STREAM.println(mcp2.digitalRead(i));
#endif
        strcat(oirBuf, OIR_X(DIGITAL_INPUT_ID));
        strcat(oirBuf, SLASH);
        oirBuf[strlen(oirBuf)] = i + '0';
        strcat(oirBuf, OIR_X(DIGITAL_INPUT_STATE));
        mcp2.digitalRead(i) ? update_event_value(context, oirBuf, "1") : update_event_value(context, oirBuf, "0");
    }
}
#endif

#ifdef LOCATION
// --- LOCATION -------------------------------------------
void updateLocation()
{
    //         //--- LOCATION -------------------------------------------
    //         sensorValue = sodaq_gps.getLat();
    //         update_sensor_value(context, "/6/0/0", sensorValue);
    //         sensorValue = sodaq_gps.getLon();
    //         update_sensor_value(context, "/6/0/1", sensorValue);
}
#endif

#ifdef LOUDNESS
// --- LOUDNESS -------------------------------------------
void updateLoudness()
{
    sensorValue = analogRead(SOUND_VALUE_PIN);
    sensorValue = roundf(sensorValue * 100) / 100;
#if defined(DEBUG_LOG) || defined(DEVELOPER_LOG)
    DEBUG_STREAM.print("LOUDNESS: ");
    DEBUG_STREAM.println(sensorValue);
#endif
    update_sensor_value(context, "/3324/0/5700", sensorValue);
}
#endif

#ifdef POWER
//--- POWER ---------------------------------------
void updatePower()
{
    sensorValue = readBattery();
    sensorValue = roundf(sensorValue * 100) / 100;
#if defined(DEBUG_LOG) || defined(DEVELOPER_LOG)
    DEBUG_STREAM.print("POWER: ");
    DEBUG_STREAM.println(sensorValue);
#endif
    update_sensor_value(context, "/3328/0/5700", sensorValue);
}
#endif

#ifdef MAGNETOMETER
// --- MAGNETOMETER ------------------------------------
void updateMagnetometer()
{
    sensors_event_t event;
    accel.getEvent(&event);
#if defined(DEBUG_LOG) || defined(DEVELOPER_LOG)
    DEBUG_STREAM.print("MAGNETOMETER: ");
    sensorValue = event.acceleration.x;
    DEBUG_STREAM.print("X=");
    DEBUG_STREAM.print(sensorValue);
    sensorValue = event.acceleration.y;
    DEBUG_STREAM.print(" Y=");
    DEBUG_STREAM.print(sensorValue);
    sensorValue = event.acceleration.z;
    DEBUG_STREAM.print("Z=");
    DEBUG_STREAM.println(sensorValue);
#endif
    sensorValue = event.acceleration.x;
    sensorValue = roundf(sensorValue * 100) / 100;
    update_sensor_value(context, "/3314/0/5702", sensorValue);
    sensorValue = event.acceleration.y;
    sensorValue = roundf(sensorValue * 100) / 100;
    update_sensor_value(context, "/3314/0/5703", sensorValue);
    sensorValue = event.acceleration.z;
    sensorValue = roundf(sensorValue * 100) / 100;
    update_sensor_value(context, "/3314/0/5704", sensorValue);

    // Calculate the angle of the vector y,x
    float heading = (atan2(event.magnetic.y, event.magnetic.x) * 180) / 3.14159;
    // Normalize to 0-360
    if (heading < 0)
    {
        heading = 360 + heading;
    }
#if defined(DEBUG_LOG) || defined(DEVELOPER_LOG)
    DEBUG_STREAM.print("COMPASS: ");
    DEBUG_STREAM.println(heading);
#endif
    update_sensor_value(context, "/3314/0/5705", heading);
}
#endif

#ifdef ACCELEROMETER
// --- ACCELEROMETER ------------------------------------
void updateAccelerometer()
{
    sensors_event_t event;
    accel.getEvent(&event);
#if defined(DEBUG_LOG) || defined(DEVELOPER_LOG)
    DEBUG_STREAM.print("ACCELEROMETER: ");
    sensorValue = event.acceleration.x;
    DEBUG_STREAM.print("X=");
    DEBUG_STREAM.print(sensorValue);
    sensorValue = event.acceleration.y;
    DEBUG_STREAM.print(" Y=");
    DEBUG_STREAM.print(sensorValue);
    sensorValue = event.acceleration.z;
    DEBUG_STREAM.print("Z=");
    DEBUG_STREAM.println(sensorValue);
#endif
    sensorValue = event.acceleration.x;
    sensorValue = roundf(sensorValue * 100) / 100;
    update_sensor_value(context, "/3313/0/5702", sensorValue);
    sensorValue = event.acceleration.y;
    sensorValue = roundf(sensorValue * 100) / 100;
    update_sensor_value(context, "/3313/0/5703", sensorValue);
    sensorValue = event.acceleration.z;
    sensorValue = roundf(sensorValue * 100) / 100;
    update_sensor_value(context, "/3313/0/5704", sensorValue);
}
#endif

#ifdef PIR
// --- PIR ---------------------------------------
void updatePIR()
{
    pinMode(PIR_PIN, INPUT);
    // --- PIR ---------------------------------------------
    delay(1000);
    DEBUG_STREAM.print("PIR: ");
    sensorValue = digitalRead(PIR_PIN);
    DEBUG_STREAM.println(sensorValue);
    sensorValue ? update_event_value(context, "/3302/0/5500", "1") : update_event_value(context, "/3302/0/5500", "0");
    // if (presence_previous_state != sensorValue && presence_previous_state)
    // {
    //     update_sensor_value_int(context, "/3302/0/5501", 1);
    //     presence_previous_state = false;
    // }
    // else if (presence_previous_state != sensorValue)
    // {
    //     presence_previous_state = true;
    // }
}
#endif

#ifdef PUSH_BUTTON
// --- PUSH_BUTTON ---------------------------------------
void updatePushButton()
{
    sensorValue = readWakeUpButton();
#if defined(DEBUG_LOG) || defined(DEVELOPER_LOG)
    DEBUG_STREAM.print("PUSH_BUTTON ");
    DEBUG_STREAM.println(ev_state == EVENT_BUTTON ? 1 : 0);
#endif
    sensorValue == 0 ? update_event_value(context, "/3347/0/5500", "1") : update_event_value(context, "/3347/0/5500", ev_state == EVENT_BUTTON ? "1" : "0");
}
#endif

#ifdef SIGNAL_STRENGTH
// --- SIGNAL_STRENGTH -------------------------------------------
void updateSignalStrength()
{
#if defined(DEBUG_LOG) || defined(DEVELOPER_LOG)
    DEBUG_STREAM.print("SIGNAL STRENGTH: ");
    DEBUG_STREAM.println(rssi);
#endif
    if (serialPort)
    {
        rssi = 99;
    }
    sprintf(rssiBuffer, "%d", rssi);
    update_event_value(context, "/10254/0/8", rssiBuffer);

    // --- MODE -------------------------------------------
#if defined(DEBUG_LOG) || defined(DEVELOPER_LOG)
    DEBUG_STREAM.print("MODE: ");
    DEBUG_STREAM.println(mode[currentState]);
#endif
    update_event_value(context, "/10254/0/6", mode[currentState]);
}
#endif
