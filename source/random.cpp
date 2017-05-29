#include <time.h>

#include "random.h"

using namespace std;

Random::Random() {
  age_file = AGE_FILE;
  ReadFile();
}

Random::Random(string s) {
  age_file = s;
  ReadFile();
}

int Random::ReadFile() {
  ifstream ifs(age_file);

  if(!ifs) { throw "Exception: file not found: " + age_file; }

  //csvファイルを1行ずつ読み込む
  total_age = 0;
  age tmp;
  string  str;
  while(getline(ifs, str)){
    string token;
    istringstream stream(str);

    getline(stream, token, ',');
    tmp.generation = stoi(token);
    getline(stream, token);
    tmp.population = stoi(token);
    tmp.start      = total_age;

    ages.push_back(tmp);
    total_age += tmp.population;
  }
  return 0;
}

int Random::getAge(int num) {
  int i = 0, r = 0, total = num % total_age;

  while (ages[i].start < total)
    ++i;

  return(ages[i].generation + num % 5);
}
