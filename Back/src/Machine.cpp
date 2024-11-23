#include "Machine.h"
Machine myMachine;
// Setters
void Machine::setStockFood(float stockFood) {
    if (stockFood == this->stockFood) return;
    this->stockFood = stockFood;
}

void Machine::setStockWater(float stockWater) {
    this->stockWater = stockWater;
}

void Machine::setServedFood(float servedFood) {
    this->servedFood = servedFood;
}

void Machine::setServedWater(float servedWater) {
    this->servedWater = servedWater;
}

void Machine::setSchedule(vector<vector<int>> schedule) {
    this->schedule = schedule;
}

// Getters
float Machine::getStockFood() {
    return stockFood;
}

float Machine::getStockWater() {
    return stockWater;
}

float Machine::getServedFood() {
    return servedFood;
}

float Machine::getServedWater() {
    return servedWater;
}

vector<vector<int>> Machine::getSchedule() {
    return schedule;
}

// Food consumption tracking methods
void Machine::updateFoodConsumption(float currentWeight) {
    if (currentWeight < previousFoodWeight) {
        float consumed = previousFoodWeight - currentWeight;
        totalFoodConsumed += consumed;
    }
    previousFoodWeight = currentWeight;
}

float Machine::getTotalFoodConsumed() {
    return totalFoodConsumed;
}

void Machine::resetFoodTracking() {
    totalFoodConsumed = 0;
}

void Machine::initializeFoodTracking(float initialFoodWeight) {
    currentFoodWeight = initialFoodWeight;
    previousFoodWeight = initialFoodWeight;
    totalFoodConsumed = 0;
}
