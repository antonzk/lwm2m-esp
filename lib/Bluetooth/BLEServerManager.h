#ifdef WITH_BLE
#ifndef BLE_DEVICE_NAME
#error "No define `BLE_DEVICE_NAME`. Define variable with device name."
#endif
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>

#define BLE_WAIT_CLIENT_TIME 30000


#define BLE_WIFI_SERVICE_UUID "97727c46-0371-11ea-8d71-362b9e155667"
class ServerCallbacks : public BLEServerCallbacks
{
public:
  void onConnect(BLEServer *pServer);
  void onDisconnect(BLEServer *pServer);
  bool isAnyConnected();

private:
  bool hasConnection;
};

////////// BLEServerManager /////////////////
class BLEServerManager
{
public:
  void begin();
  void waitingClient();
  void close();
  bool isConnected();

private:
  void initWiFiService(BLEServer *pServer);
  BLEServer *pServer;
  ServerCallbacks serverCallbacks;
};
extern BLEServerManager BLE;

#endif