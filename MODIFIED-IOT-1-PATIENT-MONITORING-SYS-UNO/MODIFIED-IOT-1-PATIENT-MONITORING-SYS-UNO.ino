#include <SoftwareSerial.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include "MAX30100.h"

#define REPORTING_PERIOD_MS     1000
PulseOximeter pox;
MAX30100 sensor;
uint32_t tsLastReport = 0;

const byte rxPin = 10;
const byte txPin = 11;
const int LM35P = A3;
int temp_val1=0;
int temp_adc_val=0;
int temp_val=0;
int temp=0;
SoftwareSerial mySerial (rxPin, txPin);
String food=" ";
String water=" ";
String medicine=" ";
String washroom=" ";
String PULSE=" ";
String OXYGEN=" ";
bool newValue = false;


void onBeatDetected() 
{
Serial.println("Beat!");
}


void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  if (!pox.begin()) 
  {
	Serial.println("FAILED");
	for(;;);
  } 
  else 
  {
	Serial.println("SUCCESS");
  }
  // Configure sensor to use 7.6mA for LED drive
	pox.setOnBeatDetectedCallback(onBeatDetected);
}
 
void loop()
{
temp_val1=0;
temp_adc_val=0;
temp_val=0;
temp=0;
    for(int i=0;i<=1000;i++)
    {
    temp_adc_val = analogRead(LM35P);	/* Read Temperature */
    temp_val1=temp_val1+temp_adc_val;
    }
    temp_val1=temp_val1/1000;
    temp_val = (temp_val1 * (5.0 / 1024.0));	/* Convert adc value to equivalent voltage */
    temp = (temp_val*10)+2;	/* LM35 gives output of 10mv/°C */

    if(analogRead(LM35P)>=500)
    {
    food="YES";
    }
    else
    {
    food="NO";
    }
    if(analogRead(LM35P)>=500)
    {
    water="YES";
    }
    else
    {
    water="NO";
    }
    if(analogRead(LM35P)>=500)
    {
    medicine="YES";
    }
    else
    {
     medicine="NO";
    }
    pox.update();

    // Asynchronously dump heart rate and oxidation levels to the serial
    // For both, a value of 0 means "invalid"
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        Serial.print("Heart rate:");
        Serial.print(pox.getHeartRate());
        Serial.print("bpm / SpO2:");
        Serial.print(pox.getSpO2());
        Serial.println("%");

        tsLastReport = millis();
    }


    PULSE=String(pox.getHeartRate());
    OXYGEN=String(pox.getSpO2());



    Serial.print("Temperature = ");
    Serial.print(temp);
    Serial.print(" Degree Celsius\n");
    delay(1000);
    Serial.println(temp_val1);
    Serial.println(temp_val);
    Serial.println(temp);
}

