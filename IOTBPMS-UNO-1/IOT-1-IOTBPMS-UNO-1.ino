#include <SoftwareSerial.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include "MAX30100.h"
#include <LiquidCrystal_I2C.h>
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

MAX30100 sensor;

#define REPORTING_PERIOD_MS     1000
PulseOximeter pox;
uint32_t tsLastReport = 0;
const byte LED=13;
const byte rxPin = 10;
const byte txPin = 11;
const byte food1 = 2;
const byte water1 = 3;
const byte medicine1 = 4;
const byte washroom1 = 5;
const byte buzzer=6;
///const int LM35P = A3;
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
    pinMode(food1,OUTPUT);
    pinMode(water1,OUTPUT);
    pinMode(medicine1,OUTPUT);
    pinMode(washroom1,OUTPUT);
    pinMode(LED,OUTPUT);
    pinMode(buzzer,OUTPUT);

    digitalWrite(food1,HIGH);
    digitalWrite(water1,HIGH);
    digitalWrite(medicine1,HIGH);
    digitalWrite(washroom1,HIGH);
    digitalWrite(buzzer,LOW);


    digitalWrite(LED,LOW);
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif
  accelgyro.initialize();
    // if (!pox.begin()) 
    // {
        
    //     for(;;);
    // } 
    // else 
    // {
 
    //     digitalWrite(LED,HIGH);
    // }
    // pox.setOnBeatDetectedCallback(onBeatDetected);

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
                       if(mySerial.available()>0)
                       {
                          String Readdata=mySerial.readString();
                            String datastr[3];
                            int StringCount = 0;
                            //Serial.println(Readdata);
                                while (Readdata.length() > 0)
                                {
                                  int index = Readdata.indexOf(',');
                                  if (index == -1) // No space found
                                    {
                                      datastr[StringCount++] = Readdata;
                                      break;
                                    }
                                  else
                                    {
                                    datastr[StringCount++] = Readdata.substring(0, index);
                                    Readdata = Readdata.substring(index+1);
                                    }
                                }
                                PULSE=String(datastr[0]);
                                OXYGEN=String(datastr[1]);
                                TEMPARATURE=String(datastr[2]);
                                // Show the resulting substrings
                                  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
                                    if((ax>16000)|| (ax<-15000) )
                                    {
                                      falled="FALLEN"; //Serial.println("fall");
                                      lcd.clear();
                                  
                                  digitalWrite(buzzer,HIGH);
                                  lcd.setCursor(0,0);
                                  lcd.print("PERSON FALLED"); 
                                  delay(2000); 
                                    }

                                    else
                                    {
                                      falled="ACTIVE";//Serial.println("active");
                                    }
                                    //delay(1000);
                                    if(analogRead(A0)>=900)
                                    {
                                    food="YES";
                                    lcd.clear();
                                  digitalWrite(food1,LOW);
                                  
                                  lcd.setCursor(0,0);
                                  lcd.print("NEED FOOD"); 
                                  delay(2000);
                                    }
                                    else
                                    {
                                    food="NO";
                                    }
                                    if(analogRead(A3)>=900)
                                    {
                                    water="YES";
                                    lcd.clear();
                                   digitalWrite(water1,LOW);
                                  
                                  lcd.setCursor(0,0);
                                  lcd.print("NEED WATER"); 
                                  delay(2000);
                                    }
                                    else
                                    {
                                    water="NO";
                                    }
                                    if(analogRead(A1)>=900)
                                    {
                                    medicine="YES";
                                    lcd.clear();
                                  digitalWrite(medicine1,LOW);
                                  
                                  lcd.setCursor(0,0);
                                  lcd.print("NEED MEDICINE"); 
                                  delay(2000);
                                    }
                                    else
                                    {
                                    medicine="NO";
                                    }
                                    if(analogRead(A2)>=900)
                                    {
                                    washroom="YES";
                                    lcd.clear();
                                  digitalWrite(washroom1,LOW);
                                  
                                  lcd.setCursor(0,0);
                                  lcd.print("WANT TO GO WASHROOM"); 
                                  delay(2000);
                                    }
                                    else
                                    {
                                    washroom="NO";
                                    }
                                    digitalWrite(LED,HIGH);
                                  
                                  lcd.clear();
                                  delay(1000);
                                  
                                  lcd.setCursor(0,0);
                                  lcd.print("HR:"+PULSE);   
                                
                                lcd.setCursor(8,0);
                                  lcd.print("SPO2:"+OXYGEN);  
                               
                                  lcd.setCursor(0,1);
                                  lcd.print("BTEMP:"+TEMPARATURE);
                                  
                                   // mySerial.println(PULSE+","+OXYGEN+","+TEMPARATURE+","+food+","+water+","+medicine+","+washroom+","+falled+",");
                                    Serial.println(PULSE+","+OXYGEN+","+TEMPARATURE+","+food+","+water+","+medicine+","+washroom+","+falled+",");
                
                digitalWrite(LED,LOW);
                }

    digitalWrite(food1,HIGH);
    digitalWrite(water1,HIGH);
    digitalWrite(medicine1,HIGH);
    digitalWrite(washroom1,HIGH);
    digitalWrite(buzzer,LOW);

//     pox.update();
//     if (millis() - tsLastReport > REPORTING_PERIOD_MS)
//     {
//         PULSE=String(pox.getHeartRate());
//         OXYGEN=String(pox.getSpO2());
//         tsLastReport = millis();
//     }


//     uint32_t tsTempSampStart = millis();
   
//     sensor.startTemperatureSampling();
//     while(!sensor.isTemperatureReady()) 
//     {
//         if (millis() - tsTempSampStart > 1000) 
//         {  
//             // Stop here
//             for(;;);
//         }
//     }

// //     float temperature = sensor.retrieveTemperature();
// //     TEMPARATURE=String(temperature);
        
}
