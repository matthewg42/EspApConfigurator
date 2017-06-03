#include "Settings.h"
#include "SettingManager.h"
#include "PersistentSettingString.h"
#include "PersistentSettingFloat.h"

PersistentSetting* settingStrProjectName;
PersistentSetting* settingFloatMagicNumber;

void initSettings()
{
    settingStrProjectName = new PersistentSettingString(0, 32);
    SettingManager.add("Project Name", settingStrProjectName);

    settingFloatMagicNumber = new PersistentSettingFloat(SettingManager.nextFreeAddress(), 3.14159);
    SettingManager.add("Magic Number", settingFloatMagicNumber);
}

