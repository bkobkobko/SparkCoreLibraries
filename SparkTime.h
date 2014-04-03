#ifndef _SPARKTIME
#define _SPARKTIME

#include "application.h"

#define SPARKTIMENTPSIZE 48
#define SPARKTIMEHOSTNAMESIZE 64
#define SPARKTIMEYEARZERO 1900
#define SPARKTIMEEPOCHSTART 2208988800UL
#define SPARKTIMESECPERDAY 86400UL
#define SPARKTIMEBASESTART 3597523200UL
#define SPARKTIMEBASEYEAR  2014UL
#define LEAPYEAR(year)  (!((year) % 4) && (((year) % 100) || !((year) % 400)))
#define YEARSIZE(year)  (LEAPYEAR(year) ? 366 : 365)
#define SPARKTIMEWRAPSECS 4294967UL

static const char _digits[60][3] = {"00","01","02","03","04","05","06","07","08","09",
			       "10","11","12","13","14","15","16","17","18","19",
			       "20","21","22","23","24","25","26","27","28","29",
			       "30","31","32","33","34","35","36","37","38","39",
			       "40","41","42","43","44","45","46","47","48","49",
			       "50","51","52","53","54","55","56","57","58","59"}; /* for speed! */
static const char _days[7][10] = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
static const char _days_short[7][4] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
static const char _months[12][10] = {"January","February","March","April","May","June","July","August","September","October","November","December"};
static const char _months_short[12][4] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
static const uint8_t _monthLength[2][12] = {
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}}; // Leap year
  // for years 2014-2036 Day in March or November
  //                             2014 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36
static const uint8_t _usDSTStart[23] = { 9, 8,13,12,11,10, 8,14,13,12,10, 9, 8,14,12,11,10, 9,14,13,12,11, 9};
static const uint8_t _usDSTEnd[23]   = { 2, 1, 6, 5, 4, 3, 1, 7, 6, 5, 3, 2, 1, 7, 5, 4, 3, 2, 7, 6, 5, 4, 2};
static const uint8_t _EuDSTStart[23] = {30,29,27,26,25,31,29,28,27,26,31,30,29,28,26,25,31,30,28,27,26,25,30};
static const uint8_t _EuDSTEnd[23]   = {26,25,30,29,28,27,25,31,30,29,27,26,25,31,29,28,27,26,31,30,29,28,26};

class SparkTime {
public:
  SparkTime();
  void begin(UDP * UPDClient);
  void begin(UDP * UPDClient, const char * NTPServer);
  bool hasSynced();
  void setNTPInvterval(uint32_t intervalMinutes);
  void setTimeZone(int32_t hoursOffset);
  void setUseDST(bool value);
  void setUseEuroDSTRule(bool value);

  uint32_t now();
  uint32_t nowNoUpdate();
  uint32_t nowEpoch();
  uint32_t lastNTPTime();

  uint8_t hour(uint32_t tnow);
  uint8_t minute(uint32_t tnow);
  uint8_t second(uint32_t tnow);
  uint8_t dayOfWeek(uint32_t tnow);
  uint8_t day(uint32_t tnow);
  uint8_t month(uint32_t tnow);
  uint32_t year(uint32_t tnow);
  
  bool isUSDST(uint32_t tnow); //2nd Sun in Mar to 1st Sun in Nov
  bool isEuroDST(uint32_t tnow); //Last Sun in Mar to last Sun in Oct

  String hourString(uint32_t tnow);
  String hour12String(uint32_t tnow);
  String minuteString(uint32_t tnow);
  String secondString(uint32_t tnow);
  String AMPMString(uint32_t tnow);
  String dayOfWeekShortString(uint32_t tnow);
  String dayOfWeekString(uint32_t tnow);
  String dayString(uint32_t tnow);
  String monthString(uint32_t tnow);
  String monthNameShortString(uint32_t tnow);
  String monthNameString(uint32_t tnow);
  String yearShortString(uint32_t tnow);
  String yearString(uint32_t tnow);
  String ISODateString(uint32_t tnow);
  String ISODateUTCString(uint32_t tnow);
 
private:
  UDP * _UDPClient;
  char _serverName[SPARKTIMEHOSTNAMESIZE];
  bool _syncedOnce = false;
  bool _isSyncing = false;
  uint32_t _lastSyncMillisTime;
  uint32_t _lastSyncNTPTime;
  uint32_t _lastSyncNTPFrac;
  uint32_t _interval;
  int32_t _timezone;
  bool _useDST;
  bool _useEuroDSTRule;
  uint32_t _localPort;
  uint8_t _packetBuffer[SPARKTIMENTPSIZE];
  void updateNTPTime();
  int32_t timeZoneDSTOffset(uint32_t tnow);
};

#endif
