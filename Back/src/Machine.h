#ifndef MACHINE_H
#define MACHINE_H

#include <vector>
using namespace std;

class Machine
{
private:
  // Stock levels and serving amounts
  float stockFood = 0;
  float stockWater = 0;
  float servedFood = 0;
  float servedWater = 0;

  // Food tracking variables
  float totalFoodConsumed = 0;
  float currentFoodWeight = 0;
  float previousFoodWeight = 0;

  // Feeding schedule [minute, hour]
  vector<vector<int>> schedule = {{0, 8}, {0, 16}, {0, 22}};

public:
  // Setters
  void setStockFood(float stockFood);
  void setStockWater(float stockWater);
  void setServedFood(float servedFood);
  void setServedWater(float servedWater);
  void setSchedule(vector<vector<int>> schedule);

  // Getters
  float getStockFood();
  float getStockWater();
  float getServedFood();
  float getServedWater();
  vector<vector<int>> getSchedule();

  // Food consumption tracking methods
  void updateFoodConsumption(float currentWeight);
  float getTotalFoodConsumed();
  void resetFoodTracking();
  void initializeFoodTracking(float initialFoodWeight);
};

extern Machine myMachine;

#endif // MACHINE_H
