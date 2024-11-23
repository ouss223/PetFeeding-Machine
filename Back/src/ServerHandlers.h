#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H
#include "Machine.h" 
#include <WebServer.h>  // or <ESP32WebServer.h> for ESP32

 // Declare server globally (or use ESP32 version if needed)
extern Machine myMachine;  // Make sure Machine class is included
extern int choice; 
void handleGetDistance();
void handleSetMode();
void handleGetMode();
void handleGetInfos();
void handleSetSchedule();
void handleGetSchedule();
void handleGetWeight();
long getDistance(int num);


#endif
