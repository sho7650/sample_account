#ifndef ACCOUNT_H
#define ACCOUNT_H

#define ACCOUNTS 5000
#define DOMAIN   "example.com"
#define MAXDEF   100

using namespace std;

struct accounts {
  string last_kanji;
  string last_kana;
  string last_name;
  string first_kanji;
  string first_kana;
  string first_name;
  string prefecture;
};

class Account {
  accounts sample_accounts[ACCOUNTS];
public:
  Account(string);
  string   LastName(int);
  string   FirstName(int);
  string   mailAddress(int, int);
};

#endif
