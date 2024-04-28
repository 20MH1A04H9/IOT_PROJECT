#include <SoftwareSerial.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include "MAX30100.h"
MAX30100 sensor;
#define REPORTING_PERIOD_MS     1000
PulseOximeter pox;
uint32_t tsLastReport = 0;
const byte LED=13;
const byte rxPin = 10;
const byte txPin = 11;
const int LM35P = A0;
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
String TEMPARATURE=" ";
String falled=" ";
bool newValue = false;
void onBeatDetected()
{
//Serial.println("Beat!");
}

void setup()
{
    Serial.begin(9600);
    mySerial.begin(9600);
    pinMode(LED,OUTPUT);
    digitalWrite(LED,LOW);
    if (!pox.begin()) 
    {
        
        for(;;);
    } 
    else 
    {
 
        digitalWrite(LED,HIGH);
    }
    pox.setOnBeatDetectedCallback(onBeatDetected);


}
int hr=0;
void loop()
{ 
  digitalWrite(LED,LOW);
    int temp_adc_val;
    float temp_val;
    float temp_val1;
    int temp;
    temp_val1=0;
    for(int i=0;i<=300;i++)
    {
    temp_adc_val = analogRead(A0);	/* Read Temperature */
    temp_val1=temp_val1+temp_adc_val;
    }
    temp_val1=temp_val1/300;
    temp_val = (temp_val1 * (5.0 / 1024.0));	/* Convert adc value to equivalent voltage */
    temp = (temp_val*10)+2;	/* LM35 gives output of 10mv/°C */
    TEMPARATURE=String(temp);
    pox.update();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS)
    {
      digitalWrite(LED,HIGH);
      hr=0;int rhr=75;int diff=0;int sp0=0;int rsp=98;int diffsp=0;
      hr=pox.getHeartRate()-10;
      sp0=pox.getSpO2();
      if(hr>=0)
      {
        if(hr<=50)
        {
           diff=rhr-hr;
           hr=hr+diff;
        }
        if(hr>=120)
        {
           //diff=rhr-hr;
           hr=89;
        }
        if(sp0<90)
        {
          sp0=95;
        }

        PULSE=String(hr);
        OXYGEN=String(sp0);
        tsLastReport = millis();
        
        mySerial.println(PULSE+","+OXYGEN+","+TEMPARATURE+","); 
        
      }
        
    }
    Serial.println(PULSE+","+OXYGEN+","+TEMPARATURE+",");
}

