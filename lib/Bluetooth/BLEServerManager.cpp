#ifdef WITH_BLE
#include "BLEServerManager.h"
void BLEServerManager::begin()
{
  // Create BLE device:
  BLEDevice::init(BLE_DEVICE_NAME);

  // Create BLE server:
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());

  //init wifi service
  initWiFiService(pServer);
  //  | WIFI|
  //  | Name|
  //  | Password |
  //  |

  // run notification (advertising):
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void BLEServerManager::waitingClient()
{
  fprintf(stderr, "Start BLE waiting client loop\n");
  unsigned long waiterTime = millis();
  while (true)
  {
    fprintf(stderr, "Millis: %d BLE: %d Waiter: %d\n", millis(), BLE_WAIT_CLIENT_TIME, waiterTime);
    if (millis() > waiterTime + BLE_WAIT_CLIENT_TIME)
    {
      fprintf(stderr, "Exit BLE waiting client loop\n");
      // break;
    }
    fprintf(stderr, "Loop\n");
  }
  fprintf(stderr, "End BLE waiting client loop\n");
}

void BLEServerManager::close()
{
  BLEDevice::deinit(true);
}

////// Services//////////////////////////

void BLEServerManager::initWiFiService(BLEServer *pServer)
{
  // Create BLE service:
  BLEService *pService = pServer->createService(BLE_WIFI_SERVICE_UUID);
  fprintf(stderr, "Init wifi service with UUID: %s", pService->getUUID().toString().c_str());

  // run service:
  pService->start();
}

/////// ServerCallbacks /////////////
void ServerCallbacks::onConnect(BLEServer *pServer)
{
  hasConnection = true;
}

void ServerCallbacks::onDisconnect(BLEServer *pServer)
{
  hasConnection = false;
}

bool ServerCallbacks::isAnyConnected()
{
  return hasConnection;
}

#endif