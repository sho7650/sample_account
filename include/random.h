#ifndef RANDOM_H
#define RANDOM_H

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <vector>

#define  AGE_FILE "data/ages.csv"

using namespace std;

struct age {
  int generation;
  int population;
  int start;
};

class Random {
  string age_file;
  int total_age;
  vector <age> ages;
  int ReadFile();
public:
  Random();
  Random(string);
  int getRandom(int);
  int getAge(int);
  int getAgeGroup(int);
  int getBirthYear(int);
  int getReward(int);
};

#endif
