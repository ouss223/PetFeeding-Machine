#include <SPI.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>
#include <vector>
#include <ArduinoJson.h>
#include <RTClib.h>
#include <HX711.h>

using namespace std;
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""
#define WIFI_CHANNEL 6
// pour la simulation on suppose que les distances sont en mm pas cm
#include "Pins.h"
#include "Machine.h"
#include "ServerHandlers.h"
//use ngrok to get a public ip address to use alexa skill
//go to alexa dev change the api to what grok gave you
//ask alexa this to get the stock percentage ; Alexa, ask Pet Feeder how full is the food container
//dont forget to include alexa code and stuff in the rapport
WebServer server(80);

Servo myServo;
RTC_DS1307 rtc;

LiquidCrystal_I2C lcd(0x27, 20, 4);
HX711 scale;
int choice = 0; // 1 = normal mode, 2 = scheduled mode
unsigned long previousMillis = 0;
int calibration_factor = 420;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup(void)
{
  previousMillis = millis();
  Serial.begin(115200);

  Serial.println("Starting..m::m:mmmmm.");
  pinMode(ECHO1, INPUT);
  pinMode(TRIG1, OUTPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);
  pinMode(TRIG3, OUTPUT);
  pinMode(ECHO3, INPUT);
  pinMode(TRIG4, OUTPUT);
  pinMode(ECHO4, INPUT);
  scale.begin(DT, SCK);
  scale.set_scale(calibration_factor);
  Wire.begin(SDA_time, SCL_time);

  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }

  myMachine.setStockFood(getDistance(3));
  myMachine.setStockWater(getDistance(4));

  float initialFoodWeight = scale.get_units(10); // Get initial food weight
  myMachine.initializeFoodTracking(initialFoodWeight);
  lcd.begin(20, 4);
  delay(200);
  lcd.backlight();
  myServo.attach(SERVO);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD, WIFI_CHANNEL);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" Connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/getDistance", HTTP_GET, handleGetDistance);
  server.on("/setMode", HTTP_POST, handleSetMode);
  server.on("/getMode", HTTP_GET, handleGetMode);
  server.on("/getInfos", HTTP_GET, handleGetInfos);
  server.on("/setSchedule", HTTP_POST, handleSetSchedule);
  server.on("/getSchedule", HTTP_GET, handleGetSchedule);
  server.on("/getWeight", HTTP_GET, handleGetWeight);
  

  server.begin();
  Serial.println("HTTP server started");
}
unsigned long servoMoveStartTime = 0;
unsigned long servoDelay = 1000;
bool servoMoving = false;

// Stores the last time the action was performed
unsigned long interval = 200;

void loop(void)
{
  DateTime now = rtc.now();

  // Print the current date and time

  unsigned long currentMillis = millis();
  Serial.print(now.year(), DEC);

  server.handleClient();

  if (choice == 1)
  {
    Serial.println("Normal refill mode active");
  }
  else if (choice == 2)
  {
    Serial.println("Scheduled refill mode active");
  }
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    myMachine.setServedFood(getDistance(1));
    myMachine.setServedWater(getDistance(2));
    myMachine.setStockFood(getDistance(3));
    myMachine.setStockWater(getDistance(4));
    // print schedule
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Stock states :");
    lcd.setCursor(0, 1);
    lcd.print("Food: ");
    lcd.print(myMachine.getStockFood() / 4);
    lcd.print("%");

    lcd.setCursor(0, 2);
    lcd.print("Water: ");
    lcd.print(myMachine.getStockWater() / 4);
    lcd.print("%");

    float currentFoodWeight = scale.get_units(10);

    // Update food consumption logic (track consumed food)
    myMachine.updateFoodConsumption(currentFoodWeight);

    // Print the total food consumed today
    Serial.print("Food consumed today: ");
    Serial.println(myMachine.getTotalFoodConsumed());
    if (choice == 1)
    {
      if (myMachine.getStockFood() < 10)
      {
        if (!servoMoving)
        {
          myServo.write(180);            // Move the servo to dispense food
          servoMoveStartTime = millis(); // Start timing the servo movement
          servoMoving = true;            // Set the servo state to moving
          Serial.println("Food dispensed");
        }
      }
    }

    // Reset daily food consumption tracking if 24 hours have passed
    if (now.hour() == 0 && now.minute() == 0 && myMachine.getTotalFoodConsumed() > 0)
    {
      myMachine.resetFoodTracking(); // Reset food consumption tracking
    }
    vector<vector<int>> schedule = myMachine.getSchedule();
    if (choice == 2)
    {
      if (now.hour() == schedule[0][1] && now.minute() == schedule[0][0] || now.hour() == schedule[1][1] && now.minute() == schedule[1][0] || now.hour() == schedule[2][1] && now.minute() == schedule[2][0])
      {

        if (!servoMoving)
        {
          myServo.write(180);            // Move the servo to dispense food
          servoMoveStartTime = millis(); // Start timing the servo movement
          servoMoving = true;            // Set the servo state to moving
          Serial.println("Food dispensed");
        }
      }
    }
    if (servoMoving && (millis() - servoMoveStartTime >= servoDelay))
    {
      myServo.write(0);    // Move the servo back
      servoMoving = false; // Reset the servo state to idle
    }
  }
}