#include "ServerHandlers.h"
#include <Arduino.h>  // If you are using Arduino functions like delay, pulseIn, etc.
#include "Pins.h"


extern WebServer server;
void handleGetDistance() {
    long distance = getDistance(1);
    String jsonResponse = "{\"distance\": " + String(distance) + "}";
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", jsonResponse);
}

void handleSetMode() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    if (server.hasArg("mode")) {
        choice = server.arg("mode").toInt();
        String responseMessage = "Mode set to " + String(choice);
        server.send(200, "text/plain", responseMessage);
    } else {
        server.send(400, "text/plain", "Mode parameter is missing");
    }
}

void handleGetMode() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    
    String jsonResponse = "{\"mode\": " + String(choice) + "}";
    server.send(200, "application/json", jsonResponse);
}


long getDistance(int num) {
    if (num < 1 || num > 4) return -1;
    
    long duration, distance;
    int trigPin = trigPins[num - 1];
    int echoPin = echoPins[num - 1];
    
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;

    return distance;
}

void handleGetInfos() {
    String infos = "{\"stockFood\": " + String(myMachine.getStockFood()/4) + 
                   ", \"stockWater\": " + String(myMachine.getStockWater()/4) +
                   ", \"servedFood\": " + String(myMachine.getServedFood()/4) + 
                   ", \"servedWater\": " + String(myMachine.getServedWater()/4) + "}";
    
    server.sendHeader("Access-Control-Allow-Origin", "*");
    if (infos.length() > 0) {
        server.send(200, "application/json", infos);
    } else {
        server.send(400, "text/plain", "Error retrieving information");
    }
}

void handleSetSchedule() {
    server.sendHeader("Access-Control-Allow-Origin", "*");

    if (server.hasArg("schedule[0][hour]")) {
        vector<vector<int>> temp_schedule;
        for (int i = 0; server.hasArg(String("schedule[") + i + "][hour]"); ++i) {
            int hour = server.arg(String("schedule[") + i + "][hour]").toInt();
            int minute = server.arg(String("schedule[") + i + "][minute]").toInt();
            temp_schedule.push_back({minute, hour});
        }
        myMachine.setSchedule(temp_schedule);
        server.send(200, "text/plain", "Schedule set successfully");
    } else {
        server.send(400, "text/plain", "Schedule parameter is missing");
    }
}

void handleGetSchedule() {
    server.sendHeader("Access-Control-Allow-Origin", "*");

    String schedule = "[";
    for (int i = 0; i < 3; i++) {
        schedule += "{\"hour\": " + String(myMachine.getSchedule()[i][1]) + 
                    ", \"minute\": " + String(myMachine.getSchedule()[i][0]) + "}";
        if (i < 2) {
            schedule += ",";
        }
    }
    schedule += "]";

    if (schedule.length() > 0) {
        server.send(200, "application/json", schedule);
    } else {
        server.send(400, "text/plain", "Error retrieving schedule");
    }
}

void handleGetWeight() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    float eatenFood = myMachine.getTotalFoodConsumed();
    String jsonResponse = "{\"eatenFood\": " + String(eatenFood) + "}";

    if (eatenFood >= 0) {
        server.send(200, "application/json", jsonResponse);
    } else {
        server.send(400, "text/plain", "Error retrieving weight");
    }
}
