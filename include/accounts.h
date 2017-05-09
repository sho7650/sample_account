#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <vector>

// #define ACCOUNTS 5000
#define DOMAIN   "example.com"
#define ACC_FILE "data/sample_account.csv"
#define MAXDEF   100

using namespace std;

struct account {
  string last_kanji;
  string last_kana;
  string last_name;
  string first_kanji;
  string first_kana;
  string first_name;
  string prefecture;
};

class Account {
  vector<account> sample_accounts;
  int             total_accounts;
  string          accounts_file;
public:
  Account();
  Account(string);
  string  LastName(int);
  string  FirstName(int);
  string  mailAddress(int, int);

protected:
  int     ReadFile();
};

#endif
