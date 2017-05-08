#include <fstream>
#include <iostream>
#include <string>
#include <time.h>
#include <getopt.h>

#include "accounts.h"
#include "addresses.h"

using namespace std;

int main(int argc, char *argv[]) {
  int i, first, last, max = MAXDEF;
  srand((unsigned)time(NULL));

  bool l_opt = true;
  bool f_opt = true;
  bool m_opt = true;
  bool a_opt = true;
  bool t_opt = true;
  struct option longopts[] = {
    { "lastname",  no_argument, NULL, 'l' },
    { "firstname", no_argument, NULL, 'f' },
    { "mail",      no_argument, NULL, 'm' },
    { "address",   no_argument, NULL, 'a' },
    { "telehpne",  no_argument, NULL, 't' },
    { 0,           0,           0,     0  },
  };

  int opt, longindex;

  while ((opt = getopt_long(argc, argv, "lfmat", longopts, &longindex)) != -1) {
    switch(opt) {
      case 'l':
      l_opt = false;
      break;
      case 'f':
      f_opt = false;
      break;
      case 'm':
      m_opt = false;
      break;
      case 'a':
      a_opt = false;
      break;
      case 't':
      t_opt = false;
      break;
      default:
      cerr << "error" << endl;
      return false;
    }
  }

  if (argc > longindex) {
    max = atoi(argv[argc - 1]);
  }

  try {
    Account name;
    Prefecture addr;

    for (i = 0; i < max; i++) {
      first = rand();
      last  = rand();
      opt   = 0;

      if ( l_opt ) { printf("%s", name.LastName(last).c_str()); opt = 1; }
      if ( f_opt ) { printf("%c%s", opt * ',', name.FirstName(first).c_str()); opt = 1; }
      if ( m_opt ) { printf("%c%s", opt * ',', name.mailAddress(first, last).c_str()); opt = 1; }
      if ( t_opt ) { printf("%c090-%04d-%04d", opt * ',', rand()%1000, rand()%1000); opt = 1; }
      if ( a_opt ) { printf("%c%s%d-%d", opt * ',', addr.getAddress(rand()).c_str(), rand()%100, rand()%100); opt = 1; }

      printf("\n");
//      printf("%s,%s,%s,090-%04d-%04d,%s%d-%d\n",
//      name.LastName(last).c_str(), name.FirstName(first).c_str(), name.mailAddress(first, last).c_str(),
//      rand()%1000, rand()%1000,addr.getAddress(rand()).c_str(), rand()%100, rand()%100);

      //      cout << name.LastName(last) << "," << name.FirstName(first) << "," << name.mailAddress(first, last) << ",";
      //      cout << "090-" << to_string(rand() % 1000) << "-" << to_string(rand() % 1000) << ",";
      //      cout << addr.getAddress(rand()) << to_string(rand() % 100) << "-" << to_string(rand() % 100) << endl;
    }
  }
  catch (string e) {
    cerr << e << endl;
    return 1;
  }

}
