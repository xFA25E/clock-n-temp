// This arduino sketch should make a clock and measure temperature

#include <LiquidCrystal.h>

const int powerPin = 6;
const int powerSwitchPin = 2;
const int changeModePin = 3;
const int minutePin = A1;
const int hourPin = A2;
const int temperaturePin = A0;
const int RS_LCDPin = 7;
const int E_LCDPin = 8;
const int D7_LCDPin = 13;
const int D6_LCDPin = 12;
const int D5_LCDPin = 11;
const int D4_LCDPin = 10;
const int D3_LCDPin = 5;
const int D2_LCDPin = 4;
const int D1_LCDPin = 1;
const int D0_LCDPin = 0;
const int brightnessPin = 9;

int powerState = 1;
int powerStateValue;
int prevPowerStateValue = 0;
int brightnessValue = 127;
float temperature;
int sensorValue;
int changeMode = 0;
int changeModeValue;
int prevChangeModeValue = 0;
int hoursPinValue;
int minutesPinValue;
int hours = 22;
int minutes = 59;
long currentMillis;
long currentDisplayTime;

LiquidCrystal lcd(RS_LCDPin, E_LCDPin, D0_LCDPin, D1_LCDPin, D2_LCDPin, D3_LCDPin,
                  D4_LCDPin, D5_LCDPin, D6_LCDPin, D7_LCDPin);

void updateTime(int curHours, int curMinutes) {
     if (curMinutes + 1 >= 60) {
          minutes = 0;
          if (curHours + 1 >= 24) {
               hours = 0;
          } else {
               hours++;
          }
     } else {
          minutes++;
     }
}

float calcTemperature(int value) {
     float voltage = (value/1023.0) * 5.0;
     return (voltage - 0.5) * 100.0;
}

void displayNormalModeLCD(int curHours, int curMinutes, float curTemperature) {
     char timeAndTemp[17];
     // char tempBuff[6];
     // dtostrf(curTemperature, 4, 2, tempBuff);
     int icurTemp = (int) curTemperature;
     sprintf(timeAndTemp, "%02d:%02d       %d C",
             curHours, curMinutes, icurTemp);

     lcd.clear();
     lcd.setCursor(0, 0);
     lcd.print("Time        Temp");
     lcd.setCursor(0, 1);
     lcd.print(timeAndTemp);
}

 void displayChangeModeLCD(int curHours, int curMinutes) {
      char timeAndTemp[17];
      sprintf(timeAndTemp, "%02d:%02d", curHours, curMinutes);
 
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Change time");
      lcd.setCursor(0, 1);
      lcd.print(timeAndTemp);
 }

void setup() {
     pinMode(powerPin, OUTPUT);
     pinMode(powerSwitchPin, INPUT);
     pinMode(changeModePin, INPUT);
     digitalWrite(powerPin, HIGH);
     analogWrite(brightnessPin, brightnessValue);
     lcd.begin(16, 2);
     currentMillis = millis();
     currentDisplayTime = millis();
}

void loop() {
     if (millis() - currentMillis >= 60000) {
          currentMillis = millis();
          updateTime(hours, minutes);
     }

     powerStateValue = digitalRead(powerSwitchPin);
     if (prevPowerStateValue != powerStateValue) {
          prevPowerStateValue = powerStateValue;
          if (powerStateValue == HIGH) {
               powerState = !powerState;
          }
     }

     if (powerState) {
          digitalWrite(powerPin, HIGH);
          // analogWrite(brightnessPin, brightnessValue);

          sensorValue = analogRead(temperaturePin);
          temperature = calcTemperature(sensorValue);

          changeModeValue = digitalRead(changeModePin);
          if (changeModeValue != prevChangeModeValue) {
               prevChangeModeValue = changeModeValue;
               if(changeModeValue == HIGH) {
                    changeMode = !changeMode;
               }
          }

          if (changeMode) {
               if (millis() - currentDisplayTime >= 1000) {
                    int minuteValue = analogRead(minutePin);
                    int hourValue = analogRead(hourPin);
                    hours = map(hourValue, 0, 1023, 0, 23);
                    minutes = map(minuteValue, 0, 1023, 0, 59);
                    displayChangeModeLCD(hours, minutes);
               }
          } else {
               if (millis() - currentDisplayTime >= 1000) {
                    currentDisplayTime = millis();
                    displayNormalModeLCD(hours, minutes, temperature);
               }
          }

     } else {
          digitalWrite(powerPin, LOW);
          // analogWrite(brightnessPin, 0);
     }
}