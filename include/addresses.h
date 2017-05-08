#ifndef ADDRESS_H
#define ADDRESS_H

#include <vector>

#define PREFECTURES  47
#define PREF_FILE    "./data/prefectures.csv"
#define ADDRESS_FILE "./data/address.csv"

using namespace std;

struct prefecture {
  int    number;
  string name;
  int    population;
  int    zips;
};

struct address {
  int    number;
  string prefecture;
  string ward;
  string city;
};

class Prefecture {
  string     prefectures_file;
  string     address_file;
  int        sum_population;
  int        sum_addresses;
  prefecture sample_prefectures[PREFECTURES];
  vector <address> sample_addresses;

public:
  Prefecture();
  Prefecture(string);
  Prefecture(string, string);
  string   getName(int);
  int      getPopulation(int);
  int      getTotal();
  string   getAddress(int);
  int      getAddresses();
  int      getZips(int);
  int      ReadFile();
};

#endif
