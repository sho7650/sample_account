#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "accounts.h"

using namespace std;

Account::Account() {
  accounts_file = ACC_FILE;
  ReadFile();
}

Account::Account(string s) {
  accounts_file = s;
  ReadFile();
}

int Account::ReadFile() {
  ifstream ifs(accounts_file);

  if(!ifs) { throw "Exception: file not found: " + accounts_file; }

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
  return 0;
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
