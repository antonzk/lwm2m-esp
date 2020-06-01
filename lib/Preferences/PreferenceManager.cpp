#ifdef WITH_PREFERENCE
#include "PreferenceManager.h"

bool PreferenceManager::begin(bool readonly = true)
{
  return preferences.begin(PREFERENCE_NAMESPACE, readonly);
}

void PreferenceManager::putString(const char *key, String data)
{
  preferences.putString(key, data);
}

String PreferenceManager::getString(const char *key)
{
  return preferences.getString(key);
}

void PreferenceManager::putBool(const char *key, bool flag)
{
   preferences.putBool(key, flag);
}

bool PreferenceManager::getBool(const char *key)
{
  return preferences.getBool(key);
}

void PreferenceManager::close()
{
  preferences.end();
}
#endif