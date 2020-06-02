#include <time.h>

#include "random.h"

using namespace std;

Random::Random()
{
  age_file = AGE_FILE;
  ReadFile();
}

Random::Random(string s)
{
  age_file = s;
  ReadFile();
}

void Random::setTime(void)
{
  time_t now = time(nullptr);
  rand_time = rand() % now;
}

int Random::ReadFile()
{
  ifstream ifs(age_file);

  if (!ifs)
  {
    throw "Exception: file not found: " + age_file;
  }

  //csvファイルを1行ずつ読み込む
  total_age = 0;
  age tmp;
  string str;
  while (getline(ifs, str))
  {
    string token;
    istringstream stream(str);

    getline(stream, token, ',');
    tmp.generation = stoi(token);
    getline(stream, token);
    tmp.population = stoi(token);
    tmp.start = total_age;

    ages.push_back(tmp);
    total_age += tmp.population;
  }
  return 0;
}

int Random::getAge(int num)
{
  int i = 0, total = num % total_age;

  while (ages[i].start < total)
    ++i;

  return (ages[i].generation + num % 5);
}

int Random::getAgeGroup(int num)
{
  int i = 0, r = 0, total = num % total_age;

  while (ages[i].start < total)
    ++i;

  r = ages[i].generation / 10;

  return (r *= 10);
}

int Random::getBirthYear(int num)
{
  int age = getAge(num);

  time_t now = time(nullptr);
  const tm *lt = localtime(&now);

  return (lt->tm_year + 1900 - age);
}

int Random::getRandomYear(void)
{
  const tm *lt = localtime(&rand_time);
  return (lt->tm_year + 1900);
}

int Random::getRandomMonth(void)
{
  const tm *lt = localtime(&rand_time);
  return (lt->tm_mon + 1);
}

int Random::getRandomDay(void)
{
  const tm *lt = localtime(&rand_time);
  return (lt->tm_mday);
}

int Random::getReward(int num)
{
  return ((50 - abs(getAgeGroup(num) - 50) + (rand() % 5)) * (rand() % 3 + 1) * 100000);
}
