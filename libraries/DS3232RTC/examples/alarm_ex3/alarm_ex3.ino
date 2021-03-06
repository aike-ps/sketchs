/*-----------------------------------------------------------------------------*
 * DS3231/DS3232 Alarm Example #3                                              *
 *                                                                             *
 * Set Alarm 1 to occur every 10 seconds.                                      *
 * Detect the alarm by polling the RTC alarm flag.                             *
 * Note that the RTC does not have an alarm mode for every 10 seconds, so      *
 * after an alarm occurs, we reset the alarm register to the current           *
 * time plus ten seconds.                                                      *
 *                                                                             *
 * Hardware:                                                                   *
 * Arduino Uno, DS3231 RTC.                                                    *
 * Connect RTC SDA to Arduino pin A4.                                          *
 * Connect RTC SCL to Arduino pin A5.                                          *
 *                                                                             *
 * Jack Christensen 16Sep2017                                                  *
 *-----------------------------------------------------------------------------*/
 
#include <DS3232RTC.h>        //http://github.com/JChristensen/DS3232RTC
#include <Streaming.h>        //http://arduiniana.org/libraries/streaming/

const time_t ALARM_INTERVAL(10);    // alarm interval in seconds

void setup()
{
    Serial.begin(115200);

    // initialize the alarms to known values, clear the alarm flags, clear the alarm interrupt flags
    RTC.setAlarm(ALM1_MATCH_DATE, 0, 0, 0, 1);
    RTC.setAlarm(ALM2_MATCH_DATE, 0, 0, 0, 1);
    RTC.alarm(ALARM_1);
    RTC.alarm(ALARM_2);
    RTC.alarmInterrupt(ALARM_1, false);
    RTC.alarmInterrupt(ALARM_2, false);
    RTC.squareWave(SQWAVE_NONE);

    tmElements_t tm;
    tm.Hour = 00;                   // set the RTC to an arbitrary time
    tm.Minute = 00;
    tm.Second = 00;
    tm.Day = 16;
    tm.Month = 9;
    tm.Year = 2017 - 1970;          // tmElements_t.Year is the offset from 1970
    time_t t = makeTime(tm);        // change the tm structure into time_t (seconds since epoch)
    time_t alarmTime = t + ALARM_INTERVAL;    // calculate the alarm time
    
    // set the current time
    RTC.set(t);
    // set the alarm
    RTC.setAlarm(ALM1_MATCH_HOURS, second(alarmTime), minute(alarmTime), hour(alarmTime), 0);    
    // clear the alarm flag
    RTC.alarm(ALARM_1);

    Serial << millis() << " Start ";
    printDateTime(RTC.get());
    Serial << endl;
}

void loop()
{
    // check to see if the alarm flag is set (also resets the flag if set)
    if ( RTC.alarm(ALARM_1) )
    {
        // get the current time
        time_t t = RTC.get();
        // calculate the next alarm time
        time_t alarmTime = t + ALARM_INTERVAL;
        // set the alarm
        RTC.setAlarm(ALM1_MATCH_HOURS, second(alarmTime), minute(alarmTime), hour(alarmTime), 0);    

        Serial << millis() << " ALARM_1 ";
        printDateTime(t);
        Serial << endl;
    }
}

void printDateTime(time_t t)
{
    Serial << ((day(t)<10) ? "0" : "") << _DEC(day(t));
    Serial << monthShortStr(month(t)) << _DEC(year(t)) << ' ';
    Serial << ((hour(t)<10) ? "0" : "") << _DEC(hour(t)) << ':';
    Serial << ((minute(t)<10) ? "0" : "") << _DEC(minute(t)) << ':';
    Serial << ((second(t)<10) ? "0" : "") << _DEC(second(t));
}

