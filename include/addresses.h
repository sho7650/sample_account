#ifndef ADDRESS_H
#define ADDRESS_H

#include <string>
#include <vector>

#define PREFECTURES  47
#define PREF_FILE    "./data/prefectures.csv"
#define ADDRESS_FILE "./data/address.csv"

struct prefecture {
  int         number;
  std::string name;
  int         population;
  int         zips;
};

struct address {
  int         number;
  std::string prefecture;
  std::string ward;
  std::string city;
};

class Prefecture {
  std::string           prefectures_file;
  std::string           address_file;
  int                   sum_population;
  int                   sum_addresses;
  int                   prefecture_number;
  prefecture            sample_prefectures[PREFECTURES];
  std::vector<address>  sample_addresses;
  int                   getZip(int, int);

public:
  Prefecture();
  Prefecture(std::string);
  Prefecture(std::string, std::string);
  std::string getName(int);
  int         getPopulation(int);
  int         getTotal();
  int         getAddresses();
  std::string getAddress(int);
  std::string getAddress(int, int);
  std::string getPrefecture(int);
  std::string getWard(int);
  std::string getWard(int, int);
  std::string getCity(int);
  std::string getCity(int, int);
  int         setPrefNumber(int);
  int         getZips(int);
  int         ReadFile();
};

#endif
