#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H
#include "Machine.h"
#include <WebServer.h>

extern Machine myMachine;
extern int choice;
void handleSetMode();
void handleGetMode();
void handleGetInfos();
void handleSetSchedule();
void handleGetSchedule();
void handleGetWeight();
long getDistance(int num);

#endif
