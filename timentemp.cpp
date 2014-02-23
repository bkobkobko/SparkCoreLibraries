// This #include statement was automatically added by the Spark IDE.
#include "OneWire.h"

// This #include statement was automatically added by the Spark IDE.
#include "Adafruit_GFX.h"

// This #include statement was automatically added by the Spark IDE.
#include "Adafruit_LEDBackpack.h"

// This #include statement was automatically added by the Spark IDE.
#include "glcdfont.h"

// This #include statement was automatically added by the Spark IDE.
#include "SparkTime.h"

// LED Clock with NTP time keeping
UDP UDPClient;
SparkTime rtc;
Adafruit_7segment matrix = Adafruit_7segment();
OneWire one = OneWire(D2);

unsigned long currentTime;
unsigned long lastTime = 0UL;

uint8_t rom[8];
uint8_t resp[9];
int32_t scaledTemp;


void setup() {
  rtc.begin(&UDPClient, "north-america.pool.ntp.org");
  rtc.setTimeZone(-5); // gmt offset
  matrix.begin(0x70);
  matrix.print(10000,DEC); // make display show ----
  matrix.writeDisplay();
  getTemp();
  delay(500);
}


void loop() {
    currentTime = rtc.now();
    if (currentTime != lastTime) {
        uint8_t sec = rtc.second(currentTime);
        uint8_t modsec = sec%10;
        if (modsec>4 && modsec<=6) {
            showTemp();
        } else {
            showTime(currentTime);
        }
        lastTime = currentTime;
        
        if (sec==25) {
            getTemp(); //fetch new temp reading every minute
        }
    }
}

void showTime(unsigned long currentTime) {
    uint8_t min  = rtc.minute(currentTime);
    uint8_t hour = rtc.hour(currentTime);
    uint8_t blink;
    if (hour>12) { hour = hour - 12; }
    if (hour==0) { hour = 12; }
    if (hour>9) {
        matrix.writeDigitNum(0, hour/10);    
    } else {
        matrix.writeDigitRaw(0,0);
    }
    matrix.writeDigitNum(1, hour%10);
    matrix.writeDigitNum(3, min/10);
    matrix.writeDigitNum(4, min%10);
    blink = (currentTime%2)<<1;
    if (rtc.hour(currentTime)<12) {
        blink |= 0x04;
    }
    matrix.writeDigitRaw(2, blink);
        
    matrix.writeDisplay();
}

void getTemp() {
    // Get the ROM address
    one.reset();
    one.write(0x33);
    one.read_bytes(rom, 8);
    // Get the temp
    one.reset();
    one.write(0x55);
    one.write_bytes(rom,8);
    one.write(0x44);
    delay(10);
    one.reset();
    one.write(0x55);
    one.write_bytes(rom, 8);
    one.write(0xBE);
    one.read_bytes(resp, 9);
    
    byte MSB = resp[1];
    byte LSB = resp[0];
    
    int16_t intTemp = ((MSB << 8) | LSB); //using two's compliment 16-bit
    double tempC =   ((double)intTemp)/16.0;
    double tempF = (( tempC*9.0)/5.0+32.0)*10.0;
    scaledTemp =  (int32_t)tempF; // 724 means 72.4

}

void showTemp() {
    // scaledTemp 746 means 74.6
    if (scaledTemp<0) {
        matrix.writeDigitRaw(0,0x40); // minus sign
    } else {
        matrix.writeDigitRaw(0,0);        
    }

    if (scaledTemp<100) {
        matrix.writeDigitRaw(1,0);
    } else {
        matrix.writeDigitNum(1,(scaledTemp/100)%10);        
    }
    matrix.writeDigitRaw(2,0x10); //degree dot
    matrix.writeDigitNum(3,(scaledTemp/10)%10);
    matrix.writeDigitNum(4,(scaledTemp%10)); 
    matrix.writeDisplay();
}
