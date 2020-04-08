#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include "ThingSpeak.h"
#include <ArduinoJson.h>
//#define DEBUG 1 

WiFiClient  client1;
unsigned long myChannelNumber =  your channel number ;
const char * myWriteAPIKey = "";

// Fingerprint for demo URL, expires on June 2, 2021, needs to be updated well before this date
const uint8_t fingerprint[20] = {0x7b,0x00,0x29,0x73,0x45,0x13,0xa3,0xf7,0x95,0x84,0x42,0xea,0xe0,0x28,0x1b,0xf4,0x0d,0xb8,0x11,0x53};
StaticJsonDocument<400> json_string;
long cases;
long deaths;
long recovered;
long active_cases;
bool upload_to_thingspeak=false;
String payload="";      


void setup() {  
  Serial.begin(9600);
  WiFiManager wifiManager;
  wifiManager.autoConnect("AutoConnectAP");
  Serial.println("Wifi connected:..)");
  ThingSpeak.begin(client1);
}

void loop() {
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setFingerprint(fingerprint);
    HTTPClient https;

    #ifdef DEBUG  
    Serial.print("[HTTPS] begin...\n");
    #endif
    
    if (https.begin(*client, "https://corona.lmao.ninja/all"))
    {
      #ifdef DEBUG
      Serial.print("[HTTPS] GET...\n");
      #endif
      // start connection and send HTTP header
      int httpCode = https.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        #ifdef DEBUG
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
        #endif

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String new_payload = https.getString();
          if(new_payload.equals(payload))
          {
            #ifdef DEBUG
            Serial.println("data didn't change");
            #endif
          }
          else
          {
            payload=new_payload;
            Serial.println(payload);
            DeserializationError error = deserializeJson(json_string, payload);
            if (error)
            {
              
            }
            else
            {
              cases=json_string["cases"];
              deaths=json_string["deaths"];
              recovered=json_string["recovered"];
              active_cases=json_string["active"];
       
              ThingSpeak.setField(1,cases );
              ThingSpeak.setField(2,recovered );
              ThingSpeak.setField(3,active_cases);
              ThingSpeak.setField(4, deaths);  
              upload_to_thingspeak=true;          
            }
          }
        }        
      } else {
        #ifdef DEBUG
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        #endif
      }

      https.end();
    } else {
      #ifdef DEBUG
      Serial.printf("[HTTPS] Unable to connect\n");
      #endif
    }
    if(upload_to_thingspeak)
    {      
      int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
      if(x == 200){
        #ifdef DEBUG
        Serial.println("Channel update successful.");
        #endif
        upload_to_thingspeak=false;//only if upload success
      }
      else{
        #ifdef DEBUG
        Serial.println("Problem updating channel. HTTP error code " + String(x));
        #endif
      }
    }
  #ifdef DEBUG
  Serial.println("Wait 30s before next round...");
  #endif
  delay(30000);
}
