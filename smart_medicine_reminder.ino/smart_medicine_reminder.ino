#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define GREEN_LED 4
#define YELLOW_LED 5
#define RED_LED 6
#define BUZZER 7

const int reminderHours[3] = {8, 14, 20};
const int reminderMinutes[3] = {0, 0, 0};
bool reminderDone[3] = {false, false, false};

void setup() {
  Wire.begin();
  rtc.begin();
  lcd.begin(16, 2);
  lcd.backlight();

  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  lcd.setCursor(0, 0);
  lcd.print("Smart Medicine");
  lcd.setCursor(0, 1);
  lcd.print("Reminder Box");
  delay(2000);
  lcd.clear();
}

void loop() {
  DateTime now = rtc.now();

  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  if (now.hour() < 10) lcd.print('0');
  lcd.print(now.hour());
  lcd.print(":");
  if (now.minute() < 10) lcd.print('0');
  lcd.print(now.minute());

  lcd.setCursor(0, 1);
  lcd.print("Date: ");
  lcd.print(now.day());
  lcd.print("/");
  lcd.print(now.month());

  delay(1000);

  if (now.hour() == 0 && now.minute() == 0) {
    for (int i = 0; i < 3; i++) reminderDone[i] = false;
  }

  for (int i = 0; i < 3; i++) {
    if (now.hour() == reminderHours[i] && now.minute() == reminderMinutes[i] && !reminderDone[i]) {
      triggerReminder(i);
      reminderDone[i] = true;
    }
  }
}

void triggerReminder(int slot) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Take Medicine!");
  lcd.setCursor(0, 1);

  if (slot == 0) {
    lcd.print("Morning Time");
    digitalWrite(GREEN_LED, HIGH);
  } else if (slot == 1) {
    lcd.print("Afternoon Time");
    digitalWrite(YELLOW_LED, HIGH);
  } else {
    lcd.print("Evening Time");
    digitalWrite(RED_LED, HIGH);
  }

  digitalWrite(BUZZER, HIGH);
  delay(10000);
  digitalWrite(BUZZER, LOW);

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);

  lcd.clear();
}