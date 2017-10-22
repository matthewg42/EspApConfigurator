#include <EEPROM.h>
#include <MutilaDebug.h>
#include "PersistentSettingTime.h"

bool basicTimeValidatorChunk(const String& s, int8_t start, int8_t end)
{
    _DB(F("basicTimeValidatorChunk: s=\""));
    _DB(s);
    _DB(F("\" st="));
    _DB(start);
    _DB(F(" en="));
    _DBLN(end);
    for (uint8_t i=start; i<end; i++) {
        _DB(F("basicTimeValidatorChunk: @"));
        _DB(i);
        _DB('=');
        _DBLN(s[i]);
        if (s[i] < '0' || s[i] > '9') {
            _DB(F("basicTimeValidatorChunk: bad character@"));
            _DBLN(i);
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
    _DB(F("timeStrToSeconds s="));
    _DBLN(s);
    
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
            _DBF("timeStrToSeconds fail in chunk=%d\n", chunkNum);
            return -1;
        }
        hms[chunkNum] = s.substring(st,en).toInt();
        _DBF("timeStrToSeconds hms[%d] = %d\n", chunkNum, hms[chunkNum]);
        if (en >= s.length()) {
            break;
        }
        if (hms[chunkNum] > hmsMax[chunkNum]) {
            _DBF("timeStrToSeconds fail in chunk=%d c=%d max=%d\n", chunkNum, hms[chunkNum], hmsMax[chunkNum]);
            return -1;
        }
        if (++chunkNum > 2) {
            _DBLN(F("timeStrToSeconds fail too many chunks"));
            return -1;
        }
        _DB(F("timeStrToSeconds en after="));
        _DBLN(en);
        st = en + 1;
        //if (st >= s.length()) {
        //    break;
        //}
        _DB(F("timeStrToSeconds next st="));
        _DBLN(st);
    }
    long result = hms[0]*3600 + hms[1]*60 + hms[2];
    _DB(F("timeStrToSeconds result="));
    _DBLN(result);
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
    _DB(F("basicTimeValidator s="));
    _DBLN(s);
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
    String s = secondsToTimeStr(_value, _displaySeconds);
    DB(F("PersistentSettingTime::get="));
    DBLN(s);
    return s;
}

bool PersistentSettingTime::set(String newValue)
{
    DB(F("PersistentSettingTime::set="));
    DBLN(newValue);
    long newLong = timeStrToSeconds(newValue);
    if (newLong != -1) {
        _value = newLong;
        return true;
    } else {
        return false;
    }   
}

