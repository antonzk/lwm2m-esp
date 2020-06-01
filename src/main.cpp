#include <Arduino.h>

#include "client_smart_objects.h"
// #include "wifi_connection.h"

#ifdef WITH_TINYDTLS
#define PSKID "ESP32"
#define PSK "FF00FF" //HEX key
#endif

char ssid[31]={0};
char password[31]={0};

//*****
bool state;

//*****//

void setup()
{

  Serial.begin(9600);
  strcpy(ssid,WIFI_SSID);
  strcpy(password,WIFI_PASSWORD);
#ifdef WITH_PREFERENCE
  //init pref
  //writable mode
  pref.begin(false);
  if (pref.getBool(PREFERENCE_WIFI_SAVED))
  {
    strcpy(ssid,pref.getString(PREFERENCE_SSID).c_str());
    strcpy(password,pref.getString(PREFERENCE_PASSWORD).c_str());
  }
  pref.close();
#endif

#ifdef WITH_BLE
 fprintf(stdout, "BLE start\n");
  BLE.begin();
  BLE.waitingClient();
   fprintf(stdout, "BLE end\n");
  BLE.close();
#endif

  // ConnectToWiFi();
  state = Wi_Fi.initMode(WIFI_STA);
  if (state)
  {
    fprintf(stdout, "WiFi mode - STA\n");
  }
  else
  {
    fprintf(stdout, "Failed: set WiFi mode STA\n");
    //TODO delay in sleep and then reboot
    // or set WD
    return;
  }
  Wi_Fi.getAvailableWifi();
  Wi_Fi.setToConnect(ssid, password);

  Wi_Fi.connect();
  fprintf(stderr, "%s\n", Wi_Fi.getMyLocalIp().toString().c_str());
  fprintf(stderr, "%s\n", Wi_Fi.getMyMAC().c_str());
  fprintf(stderr, "Status: %s\n", Wi_Fi.isConnected() ? "connected" : "not connected");

  delay(5000);
  ///////////////////////////////////////

  LwM2m.init();
}

void loop()
{
  LwM2m.lifecycle();
  //TODO add check wifi_connection
  //TODO check socket creation
}

//TODO check test
// after a one minute of lwm2m connection
// drop wifi and try again lifecycle
