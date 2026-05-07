#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>

// #define ACCOUNTS 5000
#define MAIL_DOMAIN "example.com"
#define ACC_FILE    "data/sample_account.csv"
#define MAXDEF      100

struct account {
  std::string last_kanji;
  std::string last_kana;
  std::string last_name;
  std::string first_kanji;
  std::string first_kana;
  std::string first_name;
  std::string gender;
  std::string blood_type;
};

class Account {
  std::vector<account> sample_accounts;
  int                  total_accounts;
  std::string          accounts_file;
public:
  Account();
  Account(std::string);
  // LastName/FirstName each return TWO comma-separated CSV fields:
  // "<kanji>,<kana>". Callers (notably main()) rely on this format so the
  // emitted output expands to two columns per call.
  std::string  LastName(int);
  std::string  FirstName(int);
  std::string  mailAddress(int, int);
  std::string  getGender(int);
  std::string  getBloodType(int);

protected:
  int          ReadFile();
};

#endif
