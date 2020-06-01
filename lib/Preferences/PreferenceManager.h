#ifdef WITH_PREFERENCE
#include <Preferences.h>

#define PREFERENCE_NAMESPACE "PR_WIFI"
#define PREFERENCE_WIFI_SAVED "PR_BOOL_WIFI_SAVED"
#define PREFERENCE_SSID "PR_STR_WIFI_SSID"
#define PREFERENCE_PASSWORD "PR_STR_WIFI_PASSWORD"

class PreferenceManager
{
public:
  bool begin(bool readonly);
  void putString(const char *key, String data);
  String getString(const char *key);
  void putBool(const char *key, bool flag);
  bool getBool(const char *key);
  void close();

private:
  Preferences preferences;
};
extern PreferenceManager pref;

#endif