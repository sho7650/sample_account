#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "accounts.h"

using namespace std;

Account::Account()
{
  accounts_file = ACC_FILE;
  ReadFile();
}

Account::Account(string s)
{
  accounts_file = s;
  ReadFile();
}

int Account::ReadFile()
{
  ifstream ifs(accounts_file);

  if (!ifs)
  {
    throw "Exception: file not found: " + accounts_file;
  }

  //csvファイルを1行ずつ読み込む
  total_accounts = 0;
  account tmp;
  string str;
  while (getline(ifs, str))
  {
    string token;
    istringstream stream(str);

    getline(stream, tmp.last_kanji, ',');
    getline(stream, tmp.last_kana, ',');
    getline(stream, tmp.last_name, ',');
    getline(stream, tmp.first_kanji, ',');
    getline(stream, tmp.first_kana, ',');
    getline(stream, tmp.first_name, ',');
    getline(stream, tmp.gender, ',');
    getline(stream, tmp.blood_type);

    sample_accounts.push_back(tmp);
    ++total_accounts;
  }
  return 0;
}

string Account::LastName(int num)
{
  return (sample_accounts[num % total_accounts].last_kanji + "," + sample_accounts[num % total_accounts].last_kana);
}

string Account::FirstName(int num)
{
  return (sample_accounts[num % total_accounts].first_kanji + "," + sample_accounts[num % total_accounts].first_kana);
}

string Account::mailAddress(int first, int last)
{
  return (sample_accounts[first % total_accounts].first_name + "_" + sample_accounts[last % total_accounts].last_name + "@" + DOMAIN);
}

string Account::getGender(int num)
{
  return (sample_accounts[num % total_accounts].gender);
}

string Account::getBloodType(int num)
{
  return (sample_accounts[num % total_accounts].blood_type);
}
