#include <fstream>
#include <iostream>
#include <string>
#include <time.h>

#include "accounts.h"

#define ACCOUNTS_FILE "./data/sample_account.csv"

using namespace std;

int main(int argc, char *argv[]) {
  int i, first, last, max = MAXDEF;
  srand((unsigned)time(NULL));

    if (argc > 1) {
      max = atoi(argv[argc - 1]);
    }

  try {
    Account sample(ACCOUNTS_FILE);
    for (i = 0; i < max; i++) {
      first = rand();
      last  = rand();
      cout << sample.LastName(last) << "," << sample.FirstName(first) << "," << sample.mailAddress(first, last) << endl;
    }
  }
  catch (string e) {
    cerr << e << endl;
    return 1;
  }
}
