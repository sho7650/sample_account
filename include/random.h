#ifndef RANDOM_H
#define RANDOM_H

#include <ctime>
#include <string>
#include <vector>

#define AGE_FILE "data/ages.csv"

struct age
{
  int generation;
  int population;
  int start;
};

class Random
{
  std::string      age_file;
  int              total_age;
  std::vector<age> ages;
  int              ReadFile();
  std::time_t      rand_time;

public:
  Random();
  Random(std::string);
  void setTime(void);
  int  getRandom(int);
  int  getAge(int);
  int  getAgeGroup(int);
  int  getBirthYear(int);
  int  getRandomYear(void);
  int  getRandomMonth(void);
  int  getRandomDay(void);
  int  getReward(int);
};

#endif
