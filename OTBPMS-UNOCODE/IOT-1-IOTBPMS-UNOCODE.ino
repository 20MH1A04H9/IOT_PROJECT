#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif
LiquidCrystal_I2C lcd(0x27, 16, 2);
MPU6050 accelgyro;
int16_t ax, ay, az;
int16_t gx, gy, gz;
#define OUTPUT_READABLE_ACCELGYRO
#define LED_PIN 13
bool blinkState = false;
const byte rxPin = 2;
const byte txPin = 3;
const int LM35P = A1;
SoftwareSerial mySerial (rxPin, txPin);
void setup()
{
   #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  pinMode(LED_PIN, OUTPUT);
  lcd.begin();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("IOT BASED PATIENT");  
  lcd.setCursor(0,1);
  lcd.print(" MONTRG SYS");
  delay(3000);
  lcd.clear();


  
}
 
void loop()
{
    int temp_adc_val;
    float temp_val;
    float temp_val1;
    int temp;
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
    if((ax>16000)|| (ax<-15000) )
    {
       Serial.println("person falled");
    }
    temp_val1=0;
    for(int i=0;i<=1000;i++)
    {
    temp_adc_val = analogRead(LM35P);	/* Read Temperature */
    temp_val1=temp_val1+temp_adc_val;
    }
    temp_val1=temp_val1/1000;
    temp_val = (temp_val1 * (5.0 / 1024.0));	/* Convert adc value to equivalent voltage */
    temp = (temp_val*10)+2;	/* LM35 gives output of 10mv/°C */
    Serial.print("Temperature = ");
    Serial.print(temp);
    Serial.print(" Degree Celsius\n");
    Serial.println(analogRead(A0));
    if(digitalRead(8))Serial.println("digital working");
    delay(1000);
}
 
