/*
  Modbus-Arduino Example - Test Led (Modbus IP ESP8266)
  Control a Led on GPIO0 pin using Write Single Coil Modbus Function 
  Original library
  Copyright by André Sarmento Barbosa
  http://github.com/andresarmento/modbus-arduino

  Current version
  (c)2017 Alexander Emelianov (a.m.emelianov@gmail.com)
  https://github.com/emelianov/modbus-esp8266
*/


#include <DHT.h>;

//Constants
#define DHTPIN 14    // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE,15); //// Initialize DHT sensor for normal 16mhz Arduino


//Variables
float hum;  //Stores humidity value
float temp; //Stores temperature value


#ifdef ESP8266
 #include <ESP8266WiFi.h>
#else //ESP32
 #include <WiFi.h>
#endif
#include <ModbusIP_ESP8266.h>



//Used Pins
const int ledPin = 4; //GPIO4 = esp32cam onboard led

//Modbus Registers Offsets
const int LED_COIL = 100;
const int IREG_TEMPERATURE = 1000;  
const int IREG_HUMIDITY = 1100; 

//ModbusIP object
ModbusIP mb;

long ts;
  
void setup() {
  Serial.begin(115200);
  dht.begin();
 
  WiFi.begin("****", "****");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  mb.server();

  pinMode(ledPin, OUTPUT);
  mb.addCoil(LED_COIL);
  mb.addIreg(IREG_TEMPERATURE);
  mb.addIreg(IREG_HUMIDITY);

  ts = millis();
}
 
void loop() {
   //Call once inside loop() - all magic here
   mb.task();

   //Attach ledPin to LED_COIL register
   digitalWrite(ledPin, mb.Coil(LED_COIL));

   delay(10);


   //Do each two seconds
   if (millis() > ts + 2000) {
       ts = millis();
       
       hum = dht.readHumidity();
       temp= dht.readTemperature();
        Serial.print("Humidity: ");
        Serial.print(hum);
        Serial.print(" %, Temp: ");
        Serial.print(temp);
        Serial.println(" Celsius");

       mb.Ireg(IREG_TEMPERATURE, temp);
       mb.Ireg(IREG_HUMIDITY , hum);
    
   }
   
   /*
    //Read data and store it to variables hum and temp
    hum = dht.readHumidity();
    temp= dht.readTemperature();
    //Print temp and humidity values to serial monitor
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.println(" Celsius");
    delay(2000);
    */

   
}
