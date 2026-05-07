#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>

#include "random.h"

using namespace std;

namespace {
// Strip ASCII commas and whitespace before parsing a population field.
// Required because data/ages.csv uses thousand-separators (e.g. "4,987,706").
int parse_population(const string &s)
{
  string digits;
  digits.reserve(s.size());
  for (char c : s)
  {
    if (c >= '0' && c <= '9')
      digits.push_back(c);
  }
  return digits.empty() ? 0 : stoi(digits);
}
} // namespace

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
  // rand() returns at most ~2^31; combine two draws to span time_t's range.
  uint64_t r = (static_cast<uint64_t>(rand()) << 32) ^ static_cast<uint32_t>(rand());
  rand_time = static_cast<time_t>(r % static_cast<uint64_t>(now));
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
    tmp.population = parse_population(token);
    tmp.start = total_age;

    ages.push_back(tmp);
    total_age += tmp.population;
  }
  return 0;
}

// Find the bucket whose [start, start+population) range contains `total`.
// Returns ages.size()-1 if `total` lies past the last start (defensive).
static size_t findAgeBucket(const vector<age> &ages, int total)
{
  size_t i = 0;
  while (i + 1 < ages.size() && ages[i + 1].start <= total)
    ++i;
  return i;
}

int Random::getAge(int num)
{
  int total = num % total_age;
  size_t i = findAgeBucket(ages, total);
  return (ages[i].generation + num % 5);
}

int Random::getAgeGroup(int num)
{
  int total = num % total_age;
  size_t i = findAgeBucket(ages, total);
  int r = ages[i].generation / 10;
  return (r * 10);
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
