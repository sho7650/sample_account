#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "addresses.h"

using namespace std;

int Prefecture::getZip(int pref, int num) {
  int i = 0, r = 0;
  if (pref > r) {
    for (i = 0; i < pref; i++) {
      r += sample_prefectures[i].zips;
    }
  }
  return ((num % sample_prefectures[i].zips) + r);
}

Prefecture::Prefecture() {
  prefectures_file = PREF_FILE;
  address_file     = ADDRESS_FILE;

  ReadFile();
}

Prefecture::Prefecture(string s) {
  prefectures_file = s;
  address_file     = ADDRESS_FILE;

  ReadFile();
}

Prefecture::Prefecture(string s1, string s2) {
  prefectures_file = s1;
  address_file     = s2;

  ReadFile();
}

int Prefecture::ReadFile() {
// 都道府県データの読み込み
  ifstream ifs_pref(prefectures_file);

  if(!ifs_pref) { throw "Exception: file not found: " + prefectures_file; }

  int    i = 0;
  string str;
  sum_population = 0;

  while(getline(ifs_pref, str)){
    string token;
    istringstream stream(str);

    getline(stream, token, ',');
    sample_prefectures[i].number = stoi(token);
    getline(stream, sample_prefectures[i].name, ',');
    getline(stream, token);
    sample_prefectures[i].population = stoi(token);
    sum_population                  += stoi(token);

    ++i;
  }

// アドレスファイルの読み込み
  ifstream ifs_adr(address_file);

  if(!ifs_adr) { throw "Exception: file not found: " + prefectures_file; }

  int zip       = 0;
  int current   = 1;
  sum_addresses = 0;
  address tmp;

  while(getline(ifs_adr, str)){
    string token;
    istringstream stream(str);

    getline(stream, token, ',');
    tmp.number = stoi(token);
    getline(stream, tmp.prefecture, ',');
    getline(stream, tmp.ward, ',');
    getline(stream, tmp.city);

    sample_addresses.push_back(tmp);

    ++sum_addresses;

//    cout << tmp.number << ":";

    if (current == tmp.number) {
      ++zip;
    } else {
      sample_prefectures[current-1].zips = zip;
      zip = 0;
      ++current;
    }
  }
  sample_prefectures[current-1].zips = zip;

  return 0;
}

string Prefecture::getName(int num) {
  return(sample_prefectures[num % PREFECTURES].name);
}

int Prefecture::getPopulation(int num) {
  return(sample_prefectures[num % PREFECTURES].population);
}

int Prefecture::getTotal() {
  return(sum_population);
}

int Prefecture::setPrefNumber(int num) {
  int i, r = 0, p = num % sum_population;

  for (i = 0; i < PREFECTURES; i++) {
    r += sample_prefectures[i].population;
    if ( p < r ) { break; }
  }
  prefecture_number = i;
  return(i);
}

string Prefecture::getPrefecture(int num) {
  return(sample_prefectures[num % PREFECTURES].name);
}

int Prefecture::getZips(int num) {
  return(sample_prefectures[num % PREFECTURES].zips);
}

string Prefecture::getAddress(int num) {
  return(sample_addresses[num % sum_addresses].prefecture + sample_addresses[num % sum_addresses].ward + sample_addresses[num % sum_addresses].city);
}

string Prefecture::getAddress(int pref, int num) {
  int p = getZip(pref, num);
  return(sample_addresses[p].prefecture + sample_addresses[p].ward+sample_addresses[p].city);
}

string Prefecture::getWard(int num) {
  return(sample_addresses[num % sum_addresses].ward);
}
string Prefecture::getWard(int pref, int num) {
  return(sample_addresses[getZip(pref, num)].ward);
}

string Prefecture::getCity(int num) {
  return(sample_addresses[num % sum_addresses].city);
}
string Prefecture::getCity(int pref, int num) {
  return(sample_addresses[getZip(pref, num)].city);
}

int Prefecture::getAddresses() {
  return(sum_addresses);
}
