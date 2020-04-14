# esp8266_covid19_tracker
simple ESP8266 project to track global COVID-19 stats in real time.

Arduino UNO RX pin connected to ESP-01 TX pin. 
ESP-01 also push data to thingSpeak, that will keep working even if we remove Arduino UNO. UNO is only to display count on LCD.
If LCD don't display count, reset ESP-01 after connecting to Arduino UNO.

ThingSpeak channel: https://thingspeak.com/channels/1027684

yt : https://youtu.be/YlCtPTCAl0U

I just noticied, they changed JSON response, now there are many more paramters inside, so need to set buffer size accordingly. i'll update code when get time.  
