#include <fstream>
#include <iostream>
#include <string>
#include <time.h>
#include <getopt.h>

#include "sample_account.h"
#include "accounts.h"
#include "addresses.h"
#include "random.h"

using namespace std;

int main(int argc, char *argv[]) {
  int i, first, last, pref, ward, city, max = MAXDEF;
  srand((unsigned)time(NULL));

  bool i_opt = false;
  bool l_opt = false;
  bool f_opt = false;
  bool m_opt = false;
  bool a_opt = false;
  bool t_opt = false;
  bool p_opt = false;
  bool w_opt = false;
  bool c_opt = false;
  bool g_opt = false;
  bool b_opt = false;
  struct option longopts[] = {
    { "id",         no_argument, NULL, 'i' },
    { "lastname",   no_argument, NULL, 'l' },
    { "firstname",  no_argument, NULL, 'f' },
    { "mail",       no_argument, NULL, 'm' },
    { "address",    no_argument, NULL, 'a' },
    { "telehpne",   no_argument, NULL, 't' },
    { "prefecture", no_argument, NULL, 'p' },
    { "ward",       no_argument, NULL, 'w' },
    { "city",       no_argument, NULL, 'c' },
    { "gender",     no_argument, NULL, 'g' },
    { "blood",      no_argument, NULL, 'b' },
    { 0,            0,           0,     0  },
  };

  int opt, longindex;

  while ((opt = getopt_long(argc, argv, "ilfmatpwcgb", longopts, &longindex)) != -1) {
    switch(opt) {
      case 'i':
      i_opt = true;
      break;
      case 'l':
      l_opt = true;
      break;
      case 'f':
      f_opt = true;
      break;
      case 'm':
      m_opt = true;
      break;
      case 'a':
      a_opt = true;
      break;
      case 't':
      t_opt = true;
      break;
      case 'p':
      p_opt = true;
      break;
      case 'w':
      w_opt = true;
      break;
      case 'c':
      c_opt = true;
      break;
      case 'g':
      g_opt = true;
      break;
      case 'b':
      b_opt = true;
      break;
      default:
      cerr << "error" << endl;
      return true;
    }
  }

  if (argc > optind) {
    max = atoi(argv[argc - 1]);
  }

  try {
    Account name;
    Prefecture addr;

    for (i = 0; i < max; i++) {
      first = rand();
      last  = rand();
      pref  = addr.setPrefNumber(rand());
      ward  = rand();
      city  = rand();
      opt   = 0;

      if ( i_opt ) { printf("%i", i + 1); opt = 1; }
      if ( l_opt ) { printf("%s", name.LastName(last).c_str()); opt = 1; }
      if ( f_opt ) { printf("%c%s", opt * ',', name.FirstName(first).c_str()); opt = 1; }
      if ( m_opt ) { printf("%c%s", opt * ',', name.mailAddress(first, last).c_str()); opt = 1; }
      if ( t_opt ) { printf("%c090-%04d-%04d", opt * ',', rand()%1000, rand()%1000); opt = 1; }
      if ( p_opt ) { printf("%c%s", opt * ',', addr.getPrefecture(pref).c_str()); opt = 1; }
      if ( w_opt ) { printf("%c%s", opt * ',', addr.getWard(pref, ward).c_str()); opt = 1; }
      if ( c_opt ) { printf("%c%s", opt * ',', addr.getCity(pref, city).c_str()); opt = 1; }
      if ( g_opt ) { printf("%c%s", opt * ',', name.getGender(first).c_str()); opt = 1; }
      if ( b_opt ) { printf("%c%s", opt * ',', name.getBloodType(rand()).c_str()); opt = 1; }
      if ( a_opt ) { printf("%c%s%s%s%d-%d", opt * ',', addr.getPrefecture(pref).c_str(), addr.getWard(pref, ward).c_str(), addr.getCity(pref, city).c_str(), rand()%100, rand()%100); opt = 1; }

      if ( opt == 0 ) { printf("%i", i + 1); }

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
