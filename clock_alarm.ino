#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <virtuabotixRTC.h>
#include <RTClib.h>

#define DHTPIN 13
#define DHTTYPE DHT22 // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);
virtuabotixRTC myRTC(6, 7, 8);
LiquidCrystal_I2C lcd(0x27, 16, 2);
const int buzzer = 9;
const int buttonPin1 = 2;
const int buttonPin2 = 3;
const int buttonPin3 = 4;
const int buttonPin4 = 12;
const int buttonPin5 = 11;
//variable
float temp;

boolean buttonActive = false;
boolean longPressActive = false;

long buttonTimer = 0;
long longPressTime = 400;

int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;
int buttonState4 = 0;
int buttonState5 = 0;

int alarmHoursSet = 20;
int alarmMinutesSet = 0;

int secondsSet = 0;
int minutesSet = 0;
int hoursSet = 0;
int monthSet = 0;
int dayMonthSet = 0;
int dayWeekSet = 0;
int yearSet = 0;

int timerMin = 0;
int timerSec = 0;
int timerOn = 0;

int clicks = 0;
int clicks2 = 0;
int clicks3 = 0;
int clicks4 = 0;

String dayWeekDisplay;

int screens = 1;

bool alarmToggle = true;
bool timerToggle = true;
bool alarmToggleSet = true;

int x = 0;

unsigned long previousMillis = 0;
unsigned long interval = 1000UL;

unsigned long previousMillis1 = 0;
unsigned long interval1 = 500UL;

void setup() {
  Serial.begin(9600);
  // seconds, minutes, hours, day of the week, day of the month, month, year
  //myRTC.setDS1302Time(41, 0, 16, 1, 25, 7, 2021);
  myRTC.updateTime();

  pinMode(buzzer, OUTPUT);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT);
  pinMode(buttonPin5, INPUT);

  dayMonthSet = myRTC.dayofmonth;
  monthSet = myRTC.month;
  secondsSet = myRTC.seconds;
  minutesSet = myRTC.minutes;
  hoursSet = myRTC.hours;
  dayWeekSet = myRTC.dayofweek;
  yearSet = myRTC.year;

  int buttonState1 = 0;
  int buttonState2 = 0;
  int buttonState3 = 0;
  int buttonState4 = 0;
  int buttonState5 = 0;

  dht.begin();
  lcd.begin();
  lcd.home();
  lcd.clear();
}

void loop() {

  //screens
  screensSelect: myRTC.updateTime();
  if (screens == 1) {
    screen_1();
  } else if (screens == 2) {
    screen_2();
  } else if (screens == 3) {
    screen_3();
  } else if (screens == 4) {
    set_time();
  } else if (screens == 5) {
    set_date();
  } else if (screens == 6) {
    set_day();
  } else if (screens == 7) {
    set_year();
  } else if (screens == 8) {
    alarmOnOff();
  }
  //blink
  if (millis() - previousMillis1 > interval1) {
    previousMillis1 += interval1;
    if (screens != 1) {
      lcd.print(' ');
      lcd.print(' ');
    }
  }
  //timer
  if (millis() - previousMillis > interval) {
    timerToggle = true;
    previousMillis += interval;
    if (timerOn == 1) {

      if (timerMin == 0 && timerSec == 0) {

        timerOn = 0;
        Serial.println("timerOn");
        delay(170);
        for (int i = 0; i < 255; i++) {
          myRTC.updateTime();
          analogWrite(buzzer, i);
          buttonState1 = digitalRead(buttonPin1);
          if (buttonState1 == HIGH) {
            //timerToggle = false;
            noTone(buzzer);
            delay(170);
            break;
          }
          delay(10);
        }
        //timer buzzer
        for (int i = 255; i > 0; i--) {
          analogWrite(buzzer, i);
          myRTC.updateTime();
          buttonState1 = digitalRead(buttonPin1);
          if (buttonState1 == HIGH) {
            //timerToggle = false;
            noTone(buzzer);
            delay(170);
            break;
          }
          delay(10);
        }
        noTone(buzzer);
      } else {
        if (timerOn == 1) {
          timerSec -= 1;
          if (timerSec < 0) {
            if (timerMin != 0) {
              timerMin -= 1;
            }
            timerSec = 59;
          }
        }
      }
    }
  }
  //alarm
  while (myRTC.hours == alarmHoursSet && myRTC.minutes == alarmMinutesSet && alarmToggle && alarmToggleSet) {
    delay(170);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("It is: ");
    lcd.print(alarmHoursSet/10);
    lcd.print(alarmHoursSet%10);
    lcd.print(":");
    lcd.print(alarmMinutesSet/10);
    lcd.print(alarmMinutesSet%10);
    for (int i = 0; i < 255; i++) {
      myRTC.updateTime();
      analogWrite(buzzer, i);
      buttonState1 = digitalRead(buttonPin1);
      if (buttonState1 == HIGH) {
        alarmToggle = false;
        noTone(buzzer);
        delay(170);
        goto screensSelect;
      }
      delay(10);
    }
    //alarm buzzer
    for (int i = 255; i > 0; i--) {
      analogWrite(buzzer, i);
      myRTC.updateTime();
      buttonState1 = digitalRead(buttonPin1);
      if (buttonState1 == HIGH) {
        alarmToggle = false;
        noTone(buzzer);
        delay(170);
        goto screensSelect;
      }
      delay(10);
    }

    buttonState1 = digitalRead(buttonPin1);
    if (buttonState1 == HIGH) {
      alarmToggle = false;
      noTone(buzzer);
      delay(170);
    }
    noTone(buzzer);
  }
  if (myRTC.minutes - 1 == alarmMinutesSet) {
    alarmToggle = true;
  }

}

void fixDay() {
  switch (myRTC.dayofweek) {
  case 1:
    dayWeekDisplay = "Sun";
    break;
  case 2:
    dayWeekDisplay = "Mon";
    break;
  case 3:
    dayWeekDisplay = "Tue";
    break;
  case 4:
    dayWeekDisplay = "Wed";
    break;
  case 5:
    dayWeekDisplay = "Thu";
    break;
  case 6:
    dayWeekDisplay = "Fri";
    break;
  case 7:
    dayWeekDisplay = "Sat";
    break;
  }
}

void screen_1() {
  fixDay();
  //display time
  temp = dht.readTemperature() - 1;
  lcd.setCursor(0, 0);
  lcd.print(myRTC.hours / 10);
  lcd.print(myRTC.hours % 10);
  lcd.print(":");
  lcd.print(myRTC.minutes / 10);
  lcd.print(myRTC.minutes % 10);
  lcd.print(".");
  lcd.print(myRTC.seconds / 10);
  lcd.print(myRTC.seconds % 10);
  lcd.print("  ");
  lcd.print("T:");
  lcd.print(temp, 0);
  lcd.print((char) 223);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print(" ");
  lcd.print(dayWeekDisplay);
  lcd.print(" ");
  lcd.print(myRTC.dayofmonth / 10);
  lcd.print(myRTC.dayofmonth % 10);
  lcd.print("/");
  lcd.print(myRTC.month / 10);
  lcd.print(myRTC.month % 10);
  lcd.print("/");
  lcd.print(myRTC.year);
  lcd.print("  ");
  //next screen
  if (digitalRead(buttonPin1) == HIGH) {
    if (buttonActive == false) {
      buttonActive = true;
      buttonTimer = millis();
    }
    if ((millis() - buttonTimer > longPressTime) &&
      (longPressActive == false)) {

      longPressActive = true;
      screens = 4;
      delay(170);
    }
  } else {

    if (buttonActive == true) {

      if (longPressActive == true) {

        longPressActive = false;
      } else {

        delay(170);
        lcd.clear();
        screens = 2;
      }

      buttonActive = false;
    }
  }
}
void screen_2() {
  //select
  buttonState3 = digitalRead(buttonPin3);
  if (buttonState3 == HIGH) {
    if (x == 0) {
      // Toggle on
      x = 1;
      lcd.setCursor(3, 1);
    } else {
      // Toggle off
      lcd.setCursor(0, 1);
      x = 0;
    }
    delay(170);
  }
  // add time
  buttonState2 = digitalRead(buttonPin2);
  if (buttonState2 == HIGH) {
    alarmToggle = true;
    //hours up
    if (x == 0) {
      alarmHoursSet += 1;
      if (alarmHoursSet >= 24) {
        alarmHoursSet = 0;
      }
    }
    //minutes up
    if (x == 1) {
      alarmMinutesSet += 1;
      if (alarmMinutesSet >= 60) {
        alarmMinutesSet = 0;
      }
    }
    delay(170);
  }
  buttonState4 = digitalRead(buttonPin4);
  //hours down
  if (buttonState4 == HIGH) {
    alarmToggle = true;
    if (x == 0) {
      alarmHoursSet -= 1;
      if (alarmHoursSet < 0) {
        alarmHoursSet = 23;
      }
    }
    //minute down
    if (x == 1) {
      alarmMinutesSet -= 1;
      if (alarmMinutesSet < 0) {
        alarmMinutesSet = 59;
      }
    }
    delay(170);
  }
  //print the alarm time:
  lcd.setCursor(0, 0);
  lcd.print("Alarm Time:");
  lcd.setCursor(0, 1);
  lcd.print(alarmHoursSet / 10);
  lcd.print(alarmHoursSet % 10);
  lcd.print(":");
  lcd.print(alarmMinutesSet / 10);
  lcd.print(alarmMinutesSet % 10);
  lcd.print("  ");
  if (x == 0) {
    lcd.setCursor(0, 1);
  } else {
    lcd.setCursor(3, 1);
  }
  //next screen
  if (digitalRead(buttonPin1) == HIGH) {
    if (buttonActive == false) {
      buttonActive = true;
      buttonTimer = millis();
    }
    if ((millis() - buttonTimer > longPressTime) &&
      (longPressActive == false)) {
      longPressActive = true;
      //stop from hold click;
    }
  } else {
    if (buttonActive == true) {
      if (longPressActive == true) {
        longPressActive = false;
      } else {
        //next screen
        lcd.clear();
        screens += 1;
      }
      buttonActive = false;
    }
  }
}

void screen_3() {
  //turn on timer:
  buttonState5 = digitalRead(buttonPin5);
  if (buttonState5 == HIGH) {

    if (clicks4 == 0) {
      timerOn = 1;
      clicks4 = 1;
    } else if (clicks4 == 1) {
      timerOn = 0;
      clicks4 = 0;
    }
    delay(170);
  }
  //select min/sec
  if (timerOn != 1) {
    buttonState3 = digitalRead(buttonPin3);
    if (buttonState3 == HIGH) {
      if (clicks3 == 1) {
        clicks3 = 0;
      } else {
        clicks3 += 1;
      }
      delay(170);
    }
    buttonState2 = digitalRead(buttonPin2);
    if (buttonState2 == HIGH) {
      //min up
      if (clicks3 == 0) {
        lcd.setCursor(0, 1);
        timerMin += 1;
        lcd.setCursor(0, 1);
        if (timerMin >= 100) {
          timerMin = 0;
        }
      }
      //sec up
      if (clicks3 == 1) {
        lcd.setCursor(3, 1);
        timerSec += 1;
        lcd.setCursor(3, 1);
        if (timerSec >= 60) {
          timerSec = 0;
        }
      }
      delay(170);
    }
    buttonState4 = digitalRead(buttonPin4);
    if (buttonState4 == HIGH) {
      //min down
      if (clicks3 == 0) {
        lcd.setCursor(0, 1);
        timerMin -= 1;
        lcd.setCursor(0, 1);
        if (timerMin < 0) {
          timerMin = 99;
        }
      }
      //sec down
      if (clicks3 == 1) {
        lcd.setCursor(3, 1);
        timerSec -= 1;
        lcd.setCursor(3, 1);
        if (timerSec < 0) {
          timerSec = 59;
        }
      }
      delay(170);
    }
  }
  //print timer
  lcd.setCursor(0, 0);
  lcd.print("Timer:");
  lcd.setCursor(0, 1);
  lcd.print(timerMin / 10);
  lcd.print(timerMin % 10);
  lcd.print(":");
  lcd.print(timerSec / 10);
  lcd.print(timerSec % 10);
  lcd.print("   ");
  if (clicks3 == 0) {
    lcd.setCursor(0, 1);
  }
  if (clicks3 == 1) {
    lcd.setCursor(3, 1);
  }
  //next screen
  if (digitalRead(buttonPin1) == HIGH) {
    if (buttonActive == false) {
      buttonActive = true;
      buttonTimer = millis();
    }
    if ((millis() - buttonTimer > longPressTime) &&
      (longPressActive == false)) {
      longPressActive = true;
      //stop from hold click;
    }
  } else {
    if (buttonActive == true) {
      if (longPressActive == true) {
        longPressActive = false;
      } else {
        //next screen
        lcd.clear();
        screens = 8;
      }
      buttonActive = false;
    }
  }
}
void set_time() {
  hoursSet = myRTC.hours;
  minutesSet = myRTC.minutes;
  secondsSet = myRTC.seconds;
  //next screen
  if (digitalRead(buttonPin1) == HIGH) {
    if (buttonActive == false) {
      buttonActive = true;
      buttonTimer = millis();
    }
    if ((millis() - buttonTimer > longPressTime) &&
      (longPressActive == false)) {
      longPressActive = true;
      //stop from hold click;
    }
  } else {
    if (buttonActive == true) {
      if (longPressActive == true) {
        longPressActive = false;
      } else {
        //next screen
        lcd.clear();
        screens += 1;
      }
      buttonActive = false;
    }
  }
  if (clicks == 0) {
    lcd.setCursor(0, 1);
  }
  if (clicks == 1) {
    lcd.setCursor(3, 1);
  }
  if (clicks == 2) {
    lcd.setCursor(6, 1);
  }
  //select hours/min/sec
  buttonState3 = digitalRead(buttonPin3);
  if (buttonState3 == HIGH) {
    if (clicks == 2) {
      clicks = 0;
    } else {
      clicks += 1;
    }
    delay(170);
  }
  buttonState2 = digitalRead(buttonPin2);
  if (buttonState2 == HIGH) {
    //hours up
    if (clicks == 0) {
      lcd.setCursor(0, 1);
      hoursSet += 1;
      lcd.setCursor(0, 1);
      if (hoursSet >= 24) {
        hoursSet = 0;
      }
    }
    if (clicks == 1) {
      //min up
      lcd.setCursor(3, 1);
      minutesSet += 1;
      lcd.setCursor(3, 1);
      if (minutesSet >= 60) {
        minutesSet = 0;
      }
    }
    if (clicks == 2) {
      //sec up
      lcd.setCursor(6, 1);
      secondsSet += 1;
      lcd.setCursor(6, 1);
      if (secondsSet >= 60) {
        secondsSet = 0;
      }
    }
    delay(170);
  }
  buttonState4 = digitalRead(buttonPin4);
  if (buttonState4 == HIGH) {
    //hours down
    if (clicks == 0) {
      lcd.setCursor(0, 1);
      hoursSet -= 1;
      lcd.setCursor(0, 1);
      if (hoursSet < 0) {
        hoursSet = 23;
      }
    }
    //min down
    if (clicks == 1) {
      lcd.setCursor(3, 1);
      minutesSet -= 1;
      lcd.setCursor(3, 1);
      if (minutesSet < 0) {
        minutesSet = 59;
      }
    }
    //sec down
    if (clicks == 2) {
      lcd.setCursor(6, 1);
      secondsSet -= 1;
      lcd.setCursor(6, 1);
      if (secondsSet < 0) {
        secondsSet = 59;
      }
    }
  }
  //print set time
  lcd.setCursor(0, 0);
  lcd.print("Set Time:       ");
  lcd.setCursor(0, 1);
  lcd.print(hoursSet / 10);
  lcd.print(hoursSet % 10);
  lcd.print(":");
  lcd.print(minutesSet / 10);
  lcd.print(minutesSet % 10);
  lcd.print(":");
  lcd.print(secondsSet / 10);
  lcd.print(secondsSet % 10);
  lcd.print("        ");
  if (clicks == 0) {
    lcd.setCursor(0, 1);
  }
  if (clicks == 1) {
    lcd.setCursor(3, 1);
  }
  if (clicks == 2) {
    lcd.setCursor(6, 1);
  }
  // seconds, minutes, hours, day of the week, day of the month, month, year
  myRTC.setDS1302Time(secondsSet, minutesSet, hoursSet, dayWeekSet, dayMonthSet, monthSet, yearSet);
  delay(170);
}
void set_date() {
  dayMonthSet = myRTC.dayofmonth;
  monthSet = myRTC.month;

  //next screen
  if (digitalRead(buttonPin1) == HIGH) {
    if (buttonActive == false) {
      buttonActive = true;
      buttonTimer = millis();
    }
    if ((millis() - buttonTimer > longPressTime) &&
      (longPressActive == false)) {
      longPressActive = true;
      //stop from hold click;
    }
  } else {
    if (buttonActive == true) {
      if (longPressActive == true) {
        longPressActive = false;
      } else {
        //next screen
        lcd.clear();
        screens += 1;
      }
      buttonActive = false;
    }
  }
  //select dayOfMonth/month
  buttonState3 = digitalRead(buttonPin3);
  if (buttonState3 == HIGH) {
    if (clicks2 == 1) {
      clicks2 = 0;
    } else {
      clicks2 += 1;
    }
    delay(170);
  }
  buttonState2 = digitalRead(buttonPin2);
  if (buttonState2 == HIGH) {
    //add dayOfMonth
    if (clicks2 == 0) {

      lcd.setCursor(0, 1);
      dayMonthSet += 1;
      lcd.setCursor(0, 1);
      if (monthSet == 4 || monthSet == 6 || monthSet == 6 || monthSet == 9 || monthSet == 11) {
        if (dayMonthSet > 30) {
          dayMonthSet = 1;
        }
      } else {
        if (dayMonthSet > 31) {
          dayMonthSet = 1;
        }
      }
    }
    //add month
    if (clicks2 == 1) {
      lcd.setCursor(3, 1);
      monthSet += 1;
      lcd.setCursor(3, 1);
      if (monthSet > 12) {
        monthSet = 1;
      }
    }
    delay(170);
  }
  buttonState4 = digitalRead(buttonPin4);
  if (buttonState4 == HIGH) {
    //dayOfMonth down
    if (clicks2 == 0) {
      lcd.setCursor(0, 1);
      dayMonthSet -= 1;
      lcd.setCursor(0, 1);
      if (monthSet == 4 || monthSet == 6 || monthSet == 6 || monthSet == 9 || monthSet == 11) {
        if (dayMonthSet < 1) {
          dayMonthSet = 30;
        }
      } else {
        if (dayMonthSet < 1) {
          dayMonthSet = 31;
        }
      }
    }
    //month down
    if (clicks2 == 1) {
      lcd.setCursor(3, 1);

      if (monthSet == 1) {
        monthSet = 12;
      } else {
        monthSet -= 1;
      }
    }
    delay(170);
  }
  lcd.setCursor(0, 0);
  lcd.print("Set Date:       ");
  lcd.setCursor(0, 1);
  lcd.print(dayMonthSet / 10);
  lcd.print(dayMonthSet % 10);
  lcd.print("/");
  lcd.print(monthSet / 10);
  lcd.print(monthSet % 10);
  lcd.print("        ");

  if (clicks2 == 0) {
    lcd.setCursor(0, 1);
  }
  if (clicks2 == 1) {
    lcd.setCursor(3, 1);
  }

  // seconds, minutes, hours, day of the week, day of the month, month, year
  myRTC.setDS1302Time(secondsSet, minutesSet, hoursSet, dayWeekSet, dayMonthSet, monthSet, yearSet);
  //delay(170);
}
void set_day() {
  dayWeekSet = myRTC.dayofweek;
  buttonState1 = digitalRead(buttonPin1);
  //next screen
  if (digitalRead(buttonPin1) == HIGH) {
    if (buttonActive == false) {
      buttonActive = true;
      buttonTimer = millis();
    }
    if ((millis() - buttonTimer > longPressTime) &&
      (longPressActive == false)) {
      longPressActive = true;
      //stop from hold click;
    }
  } else {
    if (buttonActive == true) {
      if (longPressActive == true) {
        longPressActive = false;
      } else {
        //next screen
        lcd.clear();
        screens += 1;
      }
      buttonActive = false;
    }
  }

  //day up
  buttonState2 = digitalRead(buttonPin2);
  if (buttonState2 == HIGH) {
    lcd.setCursor(0, 1);
    dayWeekSet += 1;
    lcd.setCursor(0, 1);
    if (dayWeekSet > 7) {
      dayWeekSet = 7;
    }
    delay(170);
  }
  //day down
  buttonState4 = digitalRead(buttonPin4);
  if (buttonState4 == HIGH) {
    lcd.setCursor(0, 1);
    dayWeekSet -= 1;
    lcd.setCursor(0, 1);
    if (dayWeekSet < 1) {
      dayWeekSet = 30;
    }
    delay(170);
  }
  //print day
  fixDay();
  lcd.setCursor(0, 0);
  lcd.print("Set Day Of Week:    ");
  lcd.setCursor(0, 1);
  lcd.print(dayWeekDisplay);
  lcd.print("    ");
  lcd.setCursor(3, 1);
  // seconds, minutes, hours, day of the week, day of the month, month, year
  myRTC.setDS1302Time(secondsSet, minutesSet, hoursSet, dayWeekSet, dayMonthSet, monthSet, yearSet);
}
void set_year() {
  yearSet = myRTC.year;
  //next screen
  buttonState1 = digitalRead(buttonPin1);
  if (digitalRead(buttonPin1) == HIGH) {
    if (buttonActive == false) {
      buttonActive = true;
      buttonTimer = millis();
    }
    if ((millis() - buttonTimer > longPressTime) &&
      (longPressActive == false)) {
      longPressActive = true;
      //stop from hold click;
    }
  } else {
    if (buttonActive == true) {
      if (longPressActive == true) {
        longPressActive = false;
      } else {
        //next screen
        lcd.clear();
        screens = 1;
      }
      buttonActive = false;
    }
  }

  //year up
  buttonState2 = digitalRead(buttonPin2);
  if (buttonState2 == HIGH) {
    lcd.setCursor(0, 1);
    yearSet += 1;
    lcd.setCursor(0, 1);
    delay(170);
  }
  //year down
  buttonState4 = digitalRead(buttonPin4);
  if (buttonState4 == HIGH) {
    lcd.setCursor(0, 1);
    yearSet -= 1;
    lcd.setCursor(0, 1);
    delay(170);
  }
  //print year
  lcd.setCursor(0, 0);
  lcd.print("Set year:       ");
  lcd.setCursor(0, 1);
  lcd.print(yearSet);
  lcd.print("    ");
  lcd.setCursor(3, 1);
  // seconds, minutes, hours, day of the week, day of the month, month, year
  myRTC.setDS1302Time(secondsSet, minutesSet, hoursSet, dayWeekSet, dayMonthSet, monthSet, yearSet);
}
void alarmOnOff() {
  //next screen
  buttonState1 = digitalRead(buttonPin1);
  if (digitalRead(buttonPin1) == HIGH) {
    if (buttonActive == false) {
      buttonActive = true;
      buttonTimer = millis();
    }
    if ((millis() - buttonTimer > longPressTime) &&
      (longPressActive == false)) {
      longPressActive = true;
      //stop from hold click;
    }
  } else {
    if (buttonActive == true) {
      if (longPressActive == true) {
        longPressActive = false;
      } else {
        //next screen
        lcd.clear();
        screens = 1;
      }
      buttonActive = false;
    }
  }

  //alarm on/off
  buttonState2 = digitalRead(buttonPin2);
  buttonState4 = digitalRead(buttonPin4);
  if (buttonState2 == HIGH || buttonState4 == HIGH) {
    lcd.setCursor(0, 1);
    Serial.println(alarmToggleSet);
    if (alarmToggleSet) {
      alarmToggleSet = false;
    } else {
      alarmToggleSet = true;
    }
    lcd.setCursor(0, 1);
    delay(170);
  }
  //print alarmToggle
  lcd.setCursor(0, 0);
  lcd.print("Alarm on / off:        ");
  lcd.setCursor(0, 1);
  if (alarmToggleSet == true) {
    lcd.print("On");
  } else {
    lcd.print("Off");
  }
  lcd.print("    ");
  lcd.setCursor(3, 1);
}
