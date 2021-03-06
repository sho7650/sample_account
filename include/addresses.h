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
  int        prefecture_number;
  prefecture sample_prefectures[PREFECTURES];
  vector <address> sample_addresses;
  int        getZip(int, int);

public:
  Prefecture();
  Prefecture(string);
  Prefecture(string, string);
  string   getName(int);
  int      getPopulation(int);
  int      getTotal();
  int      getAddresses();
  string   getAddress(int);
  string   getAddress(int, int);
  string   getPrefecture(int);
  string   getWard(int);
  string   getWard(int, int);
  string   getCity(int);
  string   getCity(int, int);
  int      setPrefNumber(int);
  int      getZips(int);
  int      ReadFile();
};

#endif
