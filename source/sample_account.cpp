#include <fstream>
#include <iostream>
#include <string>
#include <time.h>

#include "accounts.h"
#include "addresses.h"

using namespace std;

int main(int argc, char *argv[]) {
  int i, first, last, max = MAXDEF;
  srand((unsigned)time(NULL));

  if (argc > 1) {
    max = atoi(argv[argc - 1]);
  }

  try {
    Account sample;
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

  try {
    Prefecture sample;
    for (i = 0; i < PREFECTURES; i++) {
      cout << sample.getName(i) << "," << sample.getPopulation(i) << endl;
    }
    cout << sample.getTotal() << endl;
    for (i = 0; i < sample.getAddresses(); i++) {
      cout << sample.getAddress(i) << endl;
    }
  }
  catch (string e) {
    cerr << e << endl;
    return 1;
  }

}
