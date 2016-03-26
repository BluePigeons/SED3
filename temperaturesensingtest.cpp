#pragma SPARK_NO_PREPROCESSOR

// DS18B20 Thermometer Stuff
#include "DallasTemperature.h" //SparkCore specific library for DS18B20
#include "OneWire.h" //general Arduino library for OneWire communications

#define ONE_WIRE_BUS D1
#define TEMPERATURE_PRECISION 9
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress inWaterThermometer = { 0x28, 0x2E, 0x54, 0x0A, 0x06, 0x0, 0x0, 0xFB };
DeviceAddress outWaterThermometer = { 0x28, 0xF7, 0xF0, 0x08, 0x06, 0x0, 0x0, 0x82 };
double spaInTempC = -1;
double spaOutTempC = -1;
void update18B20Temp(DeviceAddress deviceAddress, double &tempC);

// Misc Stuff
//char publishString[40];
unsigned long lastUpdate = 0;
unsigned long updateDelay = 1000; //generally needs to be at least 720ms so 1000 is definitely more than enough

void setup()
{
    // DS18B20
    sensors.begin();
    sensors.setResolution(inWaterThermometer, TEMPERATURE_PRECISION);
    sensors.setResolution(outWaterThermometer, TEMPERATURE_PRECISION);
    Spark.variable("spaITemp", &spaInTempC, DOUBLE);
    Spark.variable("spaOTemp", &spaOutTempC, DOUBLE);
    
    Serial.begin(9600);   // open serial over USB
    // On Windows it will be necessary to implement the following line:
    // Make sure your Serial Terminal app is closed before powering your Core
    // Now open your Serial Terminal, and hit any key to continue!
    while(!Serial.available()) SPARK_WLAN_Loop();
}

void loop()
{
    unsigned long now = millis();
    if((now - lastUpdate) > updateDelay)
    {
        lastUpdate = now;
         
        sensors.requestTemperatures();
        update18B20Temp(inWaterThermometer, spaInTempC);
        update18B20Temp(outWaterThermometer, spaOutTempC);
        
        Serial.print("In Temp:");
        Serial.print(spaInTempC);
        Serial.print("C - Out Temp: ");
        Serial.print(spaOutTempC);
        Serial.println("C");
        /*
        // Check if there is need for an event
        if((!atTempAnnounced) && (spaInTempC > spaTargetTemp) && (spaTargetTemp != 0.0))
        {
            Spark.publish("spa", "TargetTempAchieved");
            atTempAnnounced = true;
        }
        */

    }
}

void update18B20Temp(DeviceAddress deviceAddress, double &tempC)
{
  tempC = sensors.getTempC(deviceAddress);
}