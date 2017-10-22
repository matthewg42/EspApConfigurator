#include <EEPROM.h>
#include <MutilaDebug.h>
#include "PersistentSettingTime.h"

bool basicTimeValidatorChunk(const String& s, int8_t start, int8_t end)
{
    DB(F("basicTimeValidatorChunk: s=\""));
    DB(s);
    DB(F("\" st="));
    DB(start);
    DB(F(" en="));
    DBLN(end);
    for (uint8_t i=start; i<end; i++) {
        DB(F("basicTimeValidatorChunk: @"));
        DB(i);
        DB('=');
        DBLN(s[i]);
        if (s[i] < '0' || s[i] > '9') {
            DB(F("basicTimeValidatorChunk: bad character@"));
            DBLN(i);
            return false;
        }
    }
    return true;
}

long timeStrToSeconds(String s)
{
    // Valid time examples:
    // 13:13
    // 0:00:01
    //
    // no am/pm!
    DB(F("timeStrToSeconds s="));
    DBLN(s);
    
    const uint8_t hmsMax[3] = { 23, 59, 59 };
    uint8_t hms[3] = { 0, 0, 0 };
    uint8_t chunkNum = 0;

    int st = 0;
    int en;
    while (1) {
        // find the end of the current chunk
        en = s.indexOf(':', st);
        if (en == -1) {
            en = s.length();
        }
        if (!basicTimeValidatorChunk(s, st, en)) {
            DBF("timeStrToSeconds fail in chunk=%d\n", chunkNum);
            return -1;
        }
        hms[chunkNum] = s.substring(st,en).toInt();
        DBF("timeStrToSeconds hms[%d] = %d\n", chunkNum, hms[chunkNum]);
        if (en >= s.length()) {
            break;
        }
        if (hms[chunkNum] > hmsMax[chunkNum]) {
            DBF("timeStrToSeconds fail in chunk=%d c=%d max=%d\n", chunkNum, hms[chunkNum], hmsMax[chunkNum]);
            return -1;
        }
        if (++chunkNum > 2) {
            DBLN(F("timeStrToSeconds fail too many chunks"));
            return -1;
        }
        DB(F("timeStrToSeconds en after="));
        DBLN(en);
        st = en + 1;
        //if (st >= s.length()) {
        //    break;
        //}
        DB(F("timeStrToSeconds next st="));
        DBLN(st);
    }
    long result = hms[0]*3600 + hms[1]*60 + hms[2];
    DB(F("timeStrToSeconds result="));
    DBLN(result);
    return hms[0]*3600 + hms[1]*60 + hms[2];
}

String secondsToTimeStr(long seconds, bool showSeconds)
{
    uint8_t h, m, s;
    s = seconds % 60;
    m = ((seconds - s) / 60) % 60;
    h = (seconds - (m * 60) - s) / 3600;
    String str;
    str += h;
    str += m < 10 ? ":0" : ":";
    str += m;
    if (showSeconds) {
        str += s < 10 ? ":0" : ":";
        str += s;
    }
    return str;
}

bool basicTimeValidator(String s)
{
    DB(F("basicTimeValidator s="));
    DBLN(s);
    return timeStrToSeconds(s) >= 0;
}

PersistentSettingTime::PersistentSettingTime(uint16_t eepromAddress, String defaultValue, bool displaySeconds, validatorFunction validator) :
    // PersistentSettingAtom<long>(eepromAddress, timeStrToSeconds(defaultValue), validator)
    PersistentSettingAtom<long>(eepromAddress, 0, validator),
    _displaySeconds(displaySeconds)
{
}

bool PersistentSettingTime::load()
{
    long loaded = peek();
    DB(F("PersistentSettingTime::load peeked="));
    DBLN(loaded);
    if (loaded >= 0 && loaded < (24*3600)) {
        _value = loaded;
        return true;
    } else {
        return false;
    }
}

String PersistentSettingTime::get()
{
    String s = secondsToTimeStr(_value);
    if (!_displaySeconds) {
        s = s.substring(0, s.length() - 3);
    }
    return s;
}

bool PersistentSettingTime::set(String newValue)
{
    DB(F("PersistentSettingTime::set ="));
    DBLN(newValue);
    long newLong = timeStrToSeconds(newValue);
    if (newLong != -1) {
        _value = newLong;
        return true;
    } else {
        return false;
    }   
}

