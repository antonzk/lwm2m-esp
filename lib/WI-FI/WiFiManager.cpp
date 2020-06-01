#include "WiFiManager.h"

//TOD handle
bool WiFiManager::initMode(wifi_mode_t mode)
{
  state = WiFi.mode(mode);
  WiFi.disconnect();
  delay(100);
  return state;
}

wl_status_t WiFiManager::connect()
{
  count = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    fprintf(stdout, ".\n");
    delay(500);

    if ((++count % 16) == 0)
    {
      fprintf(stdout, " still trying to connect\n");
    }
  }
  return WiFi.status();
}

wl_status_t WiFiManager::setToConnect(const char *ssid, const char *passphrase)
{
    fprintf(stdout, "WiFi: SSID %s Password: %s \n",ssid,passphrase);
  return WiFi.begin(ssid, passphrase);
}

void WiFiManager::getAvailableWifi()
{
  count = WiFi.scanNetworks();
  if (count == 0)
  {
    fprintf(stdout, "no networks found\n");
  }
  else
  {
    fprintf(stdout, "%d networks found\n", count);
    for (int i = 0; i < count; ++i)
    {
      // Print SSID and RSSI for each network found
      fprintf(stdout, "%d: %s (%d)%c\n", i + 1, WiFi.SSID(i).c_str(), WiFi.RSSI(i),(WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? ' ': '*');
      delay(10);
    }
  }
}

bool WiFiManager::isConnected()
{

  return WiFi.isConnected();
}

IPAddress WiFiManager::getMyLocalIp()
{
  return WiFi.localIP();
}

String WiFiManager::getMyMAC()
{
  return WiFi.macAddress();
}
