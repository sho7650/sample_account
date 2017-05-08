#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "addresses.h"

using namespace std;

Prefecture::Prefecture() {
  prefectures_file = PREF_FILE;
  address_file     = ADDRESS_FILE;
}

Prefecture::Prefecture(string s) {
  prefectures_file = s;
  address_file     = ADDRESS_FILE;
}

Prefecture::Prefecture(string s1, string s2) {
  prefectures_file = s1;
  address_file     = s2;
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

  int       zip = 0;
  sum_addresses = 0;
  address   tmp;

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
  }

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

string Prefecture::getAddress(int num) {
  return(sample_addresses[num % sum_addresses].prefecture + sample_addresses[num % sum_addresses].ward + sample_addresses[num % sum_addresses].city);
}

int Prefecture::getAddresses() {
  return(sum_addresses);
}
