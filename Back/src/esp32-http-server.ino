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
#include <PubSubClient.h>

using namespace std;
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""
#define WIFI_CHANNEL 6
// pour la simulation on suppose que les distances sont en mm pas cm
#include "Pins.h"
#include "Machine.h"
#include "ServerHandlers.h"
//ask alexa this to get the stocks percentages ; Alexa, ask Pet Feeder how full is the food container
//dont forget to include alexa code and stuff in the rapport
//website for seeing mqtt messages : https://testclient-cloud.mqtt.cool

const char* mqtt_server = "broker.mqtt.cool";  
const int mqtt_port = 1883;                      
//const char* mqtt_user = "ousemaa";         
//const char* mqtt_pass = "Project9"; 

const char* mqtt_topic = "petfeeder/foodstock";
WiFiClient espClient;
PubSubClient client(espClient);



WebServer server(80);

Servo myServo;
RTC_DS1307 rtc;

LiquidCrystal_I2C lcd(0x27, 20, 4);
HX711 scale;
HX711 scale1;
HX711 scale2;

int choice = 0; // 1 = normal mode, 2 = scheduled mode
unsigned long previousMillis = 0;
int calibration_factor = 420; //done through experimenting , raw_value / known_weight , here for 100g it gave 42000


void setup(void)
{
  previousMillis = millis();
  Serial.begin(921600);

  Serial.println("Starting..m::m:mmmmm.");
  pinMode(ECHO1, INPUT);
  pinMode(TRIG1, OUTPUT);
  pinMode(TRIG3, OUTPUT);
  pinMode(ECHO3, INPUT);
  scale.begin(DT, SCK);
  scale1.begin(DT1, SCK1); // 1 is for stock
  scale2.begin(DT2, SCK2); // 2 is for the served , water density = 1  so weight is same as mass

  scale.set_scale(calibration_factor);
  scale1.set_scale(calibration_factor);
  scale2.set_scale(calibration_factor);
  
  Wire.begin(SDA_time, SCL_time);

  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }

  myMachine.setStockFood(getDistance(2));
  myMachine.setStockWater(scale1.get_units(10));

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


  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(mqttCallback);

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

// Stores the last time the action was performed(do it every "interval" time)
unsigned long interval = 2
00;

void loop(void)
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop(); 


  DateTime now = rtc.now();


  unsigned long currentMillis = millis();

  server.handleClient();


  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    myMachine.setServedFood(getDistance(1));
    myMachine.setServedWater(scale2.get_units(3));
    myMachine.setStockFood(getDistance(2));
    myMachine.setStockWater(scale1.get_units(3));


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

    lcd.print(String(myMachine.getStockWater() * 20, 2));

    lcd.print("%");

    float currentFoodWeight = scale.get_units(3);

    // Update food consumption logic (track consumed food)
    myMachine.updateFoodConsumption(currentFoodWeight);

    // Print the total food consumed today
    if (choice == 1)
    {
      if (myMachine.getServedFood() < 10)
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
    if (choice == 2)
    {
      vector<vector<int>> schedule = myMachine.getSchedule();
      if (now.hour() == schedule[0][1] && now.minute() == schedule[0][0] || now.hour() == schedule[1][1] && now.minute() == schedule[1][0] || now.hour() == schedule[2][1] && now.minute() == schedule[2][0])
      {

        if (!servoMoving)
        {
          myServo.write(180);           
          servoMoveStartTime = millis(); 
          servoMoving = true;           
          Serial.println("Food dispensed");
        }
      }
    }
    if (servoMoving && (millis() - servoMoveStartTime >= servoDelay))
    {
      myServo.write(0);   
      servoMoving = false; 
    }
  }
  publishStock(myMachine.getStockFood(),myMachine.getStockWater());//publish to broker
}



void reconnect() {
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    String clientId = "esp32-sol-clientId-";
    clientId += String(random(0xffff), HEX); //gives a random 4 chars hex string to the client id
    if (client.connect(clientId.c_str())) {
      Serial.println("Connected");
      client.subscribe("petfeeder/fooddtock");//topic we're working on
      client.subscribe("feedback");     
    } else {
      delay(5000);
    }
  }
}


void publishStock(float stockFood, float stockWater) {
  //gonna publish in json format
  String payload = "{\"food\":" + String(stockFood / 4, 2) + ",\"water\":" + (stockWater*20) + "}";
  client.publish(mqtt_topic, payload.c_str());
}
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

}