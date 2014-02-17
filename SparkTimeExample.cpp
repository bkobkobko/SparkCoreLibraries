// This #include statement was automatically added by the Spark IDE.
#include "SparkTime.h"

UDP UDPClient;
SparkTime rtc;

unsigned long currentTime;
unsigned long lastTime = 0UL;
String timeStr;

void setup() {
  rtc.begin(&UDPClient, "north-america.pool.ntp.org");
  rtc.setTimeZone(-5); // gmt offset
  Serial.begin(9600);
}

void loop() {
    currentTime = rtc.now();
    if (currentTime != lastTime) {
      byte sec = rtc.second(currentTime);
      if (sec == 10) {
	// Build Date String
	timeStr = "";
	timeStr += rtc.dayOfWeekString(currentTime);
	timeStr += ", ";
	timeStr += rtc.monthNameString(currentTime);
	timeStr += " "; 
	timeStr += rtc.dayString(currentTime);
	timeStr += ", ";
	timeStr += rtc.yearString(currentTime);
	Serial.print(timeStr);
      } else if (sec == 40) {
	// Including current timezone
	Serial.print(rtc.ISODateString(currentTime));
      } else if (sec == 50) {
	// UTC or Zulu time
	Serial.print(rtc.ISODateUTCString(currentTime));	
      } else {
	// Just the time in 12 hour format
	timeStr = "";
	timeStr += rtc.hour12String(currentTime);
	timeStr += ":";
	timeStr += rtc.minuteString(currentTime);
	timeStr += ":";
	timeStr += rtc.secondString(currentTime);	
	timeStr += " ";	
	timeStr += rtc.AMPMString(currentTime);
	Serial.print(timeStr);
      }
      lastTime = currentTime;
    }

}
