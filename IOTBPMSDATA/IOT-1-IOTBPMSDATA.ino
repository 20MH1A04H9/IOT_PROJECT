/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-mysql-database-php/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <Wire.h>


// Replace with your network credentials
const char* ssid     = "IOTBPMS";
const char* password = "12345678";

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "https://tech-guide.online/iot-1-iotbpms.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 
String apiKeyValue = "IOTBPMS";

String sensorName = "BME280";
String sensorLocation = "Office";
int NETWLED=D0;
int BULB=D4;
int BULB1=D2;
int FAN=D5;
int AC=D6;
int PUMP=D7;

#define SEALEVELPRESSURE_HPA (1013.25)
int i=0;
String  datastr = "20,OFF,32,OFF,80,ON,WET,OFF";
//Adafruit_BME280 bme(BME_CS);  // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK);  // software SPI
void dataposting(void)
{
digitalWrite(NETWLED,LOW);
delay(500);
digitalWrite(NETWLED,HIGH);
delay(50);
digitalWrite(NETWLED,LOW);
}
void setup() 
{
  Serial.begin(9600);
  pinMode(NETWLED,OUTPUT);
  digitalWrite(NETWLED,HIGH);
  WiFi.begin(ssid, password);
  Serial.println("IOT BASED PATIENT MONITORING SYSTEM");
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(NETWLED,LOW);

  // (you can also pass in a Wire library object like &Wire2)
}

void loop() 
{ 
   yield();
  //Check WiFi connection status
                    if(Serial.available()>0)
                    {
                      yield();
                            String Readdata=Serial.readString();
                            String datastr[8];
                            int StringCount = 0;
                            Serial.println(Readdata);
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

                                // Show the resulting substrings
                                for (int i = 0; i < StringCount; i++)
                                {
                                  Serial.print(i);
                                  Serial.print(": \"");
                                  Serial.print(datastr[i]);
                                  Serial.println("\"");
                                }

                                
                                if(WiFi.status()== WL_CONNECTED)
                                {
                                      std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
                                      client->setInsecure();
                                      HTTPClient https;
                                      https.begin(*client, serverName);
                                      https.addHeader("Content-Type", "application/x-www-form-urlencoded");
                                      String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + sensorName
                                                            + "&location=" + sensorLocation + "&value1=" + datastr[0]
                                                            + "&value2=" +datastr[1] + "&value3=" + datastr[2] + "&value4=" + datastr[3] +  "&value5=" + datastr[4] +  "&value6=" + datastr[5] + "&value7=" + datastr[6] + "&value8=" + datastr[7] + "&value9=" + String(i) + "&value10=" + String(i) +"";
                                      Serial.print("httpRequestData: ");
                                      Serial.println(httpRequestData);
                                      i=i+1;
                                      int httpResponseCode = https.POST(httpRequestData);   
                                                        if (httpResponseCode>0) 
                                                        {
                                                          Serial.print("HTTP Response code: ");
                                                          Serial.println(httpResponseCode);
                                                          dataposting();
                                                        }
                                                        else 
                                                        {
                                                          Serial.print("Error code: ");
                                                          Serial.println(httpResponseCode);
                                                          
                                                        }
                                                        https.end();
                                }
                                else 
                                {  
                                  Serial.println("WiFi Disconnected");
                                  digitalWrite(NETWLED,HIGH);
                                }
                      }
                    
}
   
  


            
 