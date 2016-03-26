//THIS IS BASED ON THE EXAMPLE CODE USED BY HARRISON JONES AT https://gist.github.com/harrisonhjones/3a8f81e99a40c4a28167 

//#pragma SPARK_NO_PREPROCESSOR

// DS18B20 Thermometer Stuff
#include "DallasTemperature.h" //Particle Core specific library for DS18B20
#include "OneWire.h" //Particle Core library for OneWire communications

#define ONE_WIRE_BUS D6
#define TEMPERATURE_PRECISION 12 //12 for DS18B20
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

DeviceAddress Thermometer = { 0x28, 0xF7, 0xF0, 0x08, 0x06, 0x0, 0x0, 0x82 };

double spaTempC = -1; //????

void update18B20Temp(DeviceAddress deviceAddress, double &tempC);

// Misc Stuff
//char publishString[40];
unsigned long lastUpdate = 0;
unsigned long updateDelay = 1000; //generally needs to be at least 720ms so 1000 is definitely more than enough

void setup() 
{
    // DS18B20
    sensors.begin();

    sensors.setResolution(Thermometer, TEMPERATURE_PRECISION);

    Spark.variable("spaTemp", &spaOutTempC, DOUBLE); //defines it as a variable to be addressed with GET in the CLI
    
    Serial.begin(9600);   // open serial over USB - setting speed to Particle Core standard 9600

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

        update18B20Temp(Thermometer, spaTempC); //save current reading to variable
        
        Serial.print("The Temp is: "); //message to display before reading
        Serial.print(spaTempC); //display variable value in terminal
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