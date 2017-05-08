#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "accounts.h"

using namespace std;

Account::Account(string file_name) {
  ifstream ifs(file_name);

  if(!ifs) { throw "Exception: file not found: " + file_name; }

  //csvファイルを1行ずつ読み込む
  int    i = 0;
  string str;
  while(getline(ifs, str)){
    string token;
    istringstream stream(str);

    getline(stream, sample_accounts[i].last_kanji, ',');
    getline(stream, sample_accounts[i].last_kana, ',');
    getline(stream, sample_accounts[i].last_name, ',');
    getline(stream, sample_accounts[i].first_kanji, ',');
    getline(stream, sample_accounts[i].first_kana, ',');
    getline(stream, sample_accounts[i].first_name, ',');
    getline(stream, sample_accounts[i].prefecture);

    ++i;
  }
}

string Account::LastName(int num) {
  return(sample_accounts[num % ACCOUNTS].last_kanji + "," + sample_accounts[num % ACCOUNTS].last_kana);
}

string Account::FirstName(int num) {
  return(sample_accounts[num % ACCOUNTS].first_kanji + "," + sample_accounts[num % ACCOUNTS].first_kana);
}

string Account::mailAddress(int first, int last) {
  return(sample_accounts[first % ACCOUNTS].first_name + "_" + sample_accounts[last % ACCOUNTS].last_name + "@" + DOMAIN);
}
