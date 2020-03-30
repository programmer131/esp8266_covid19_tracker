#include <ArduinoJson.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7); /// REGISTER SELECT PIN,ENABLE PIN,D4 PIN,D5 PIN, D6 PIN, D7 PIN
StaticJsonDocument<200> json_string;

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup()
{
  Serial.begin(9600);
  inputString.reserve(200);
  lcd.begin(20, 4);
  delay(100);
  lcd.setCursor(2, 1);
  lcd.print("Realtime counter");
  lcd.setCursor(3, 0);
  lcd.print("COVID-19 2020");
  lcd.setCursor(2, 2);
  lcd.print("Recovered Cases..");

}

void loop()
{
    if (stringComplete) {
    DeserializationError error = deserializeJson(json_string, inputString);
    if (error)
    {
      Serial.println(inputString);
    }
    else
    {
      String updated="";
      unsigned long cases=json_string["cases"];
      unsigned long deaths=json_string["deaths"];
      unsigned long recovered=json_string["recovered"];
      unsigned long active=json_string["active"];
      Serial.print("Recovered:");
      Serial.println(recovered);
      Serial.print("cases:");
      Serial.println(cases);
      Serial.print("deaths:");
      Serial.println(deaths);
      Serial.print("active:");
      Serial.println(active);
      lcd.setCursor(6, 4);
      lcd.print(recovered);

    }
    // clear the string:
    inputString = "";
    stringComplete = false;
  } 
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
