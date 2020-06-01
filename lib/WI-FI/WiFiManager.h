#include "WiFi.h"

#include "HardwareSerial.h"

class WiFiManager
{

public:
  bool initMode(wifi_mode_t mode);
  wl_status_t connect();

  wl_status_t setToConnect(const char *ssid, const char *passphrase);

  void getAvailableWifi(); // done partialy
  bool isConnected();      // done

  IPAddress getMyLocalIp(); //done
  String getMyMAC(); //done

private:
  int count = 0;
  bool state = false;
};

extern WiFiManager Wi_Fi;