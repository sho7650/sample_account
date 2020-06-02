#include <fstream>
#include <iostream>
#include <string>
#include <time.h>
#include <getopt.h>
#include <vector>

#include "sample_account.h"
#include "accounts.h"
#include "addresses.h"
#include "random.h"
// #include "hash.h"

using namespace std;

int main(int argc, char *argv[])
{
  int i, j, first, last, pref, ward, city, age, max = MAXDEF;
  int opts[OPTIONS], max_opts = 0;
  srand((unsigned)time(NULL));
  string key;
  int hash_seed = rand();

  struct option longopts[] = {
      {"id", no_argument, NULL, 'i'},
      {"lastname", no_argument, NULL, 'l'},
      {"firstname", no_argument, NULL, 'f'},
      {"mail", no_argument, NULL, 'm'},
      {"age", no_argument, NULL, 'a'},
      {"telehpne", no_argument, NULL, 't'},
      {"prefecture", no_argument, NULL, 'p'},
      {"ward", no_argument, NULL, 'w'},
      {"city", no_argument, NULL, 'c'},
      {"gender", no_argument, NULL, 'g'},
      {"blood", no_argument, NULL, 'b'},
      {"date", no_argument, NULL, 'd'},
      {"reward", no_argument, NULL, 'r'},
      {"random", no_argument, NULL, 'n'},
      {"quotient", no_argument, NULL, 'q'},
      {0, 0, 0, 0},
  };

  int opt, longindex;

  while ((opt = getopt_long(argc, argv, "ilfmaotpwrycgbdnq", longopts, &longindex)) != -1)
  {
    opts[max_opts++] = opt;
  }

  if (max_opts == 0)
  {
    opts[max_opts++] = 'i';
  }

  if (argc > optind)
  {
    max = atoi(argv[argc - 1]);
  }

  try
  {
    Account name;
    Prefecture addr;
    Random number;
    //    GenerateHash hash;

    for (i = 0; i < max; i++)
    {
      first = rand();
      last = rand();
      pref = addr.setPrefNumber(rand());
      ward = rand();
      city = rand();
      opt = 0;
      key = to_string(i + hash_seed);
      age = rand();

      for (j = 0; j < max_opts; j++)
      {
        if (j > 0)
        {
          printf(",");
        }

        switch (opts[j])
        {
        case 'i':
          printf("%i", i + 1);
          break;
        /*
          if (s2': { printf("%s", hash.getSHA256(key));
          if (s5': { printf("%s", hash.getSHA512(key));
          */
        case 'l':
          printf("%s", name.LastName(last).c_str());
          break;

        case 'f':
          printf("%s", name.FirstName(first).c_str());
          break;

        case 'm':
          printf("%s", name.mailAddress(first, last).c_str());
          break;

        case 't':
          printf("090-%04d-%04d", rand() % 1000, rand() % 1000);
          break;

        case 'p':
          printf("%s", addr.getPrefecture(pref).c_str());
          break;

        case 'w':
          printf("%s", addr.getWard(pref, ward).c_str());
          break;

        case 'c':
          printf("%s", addr.getCity(pref, city).c_str());
          break;

        case 'g':
          printf("%s", name.getGender(first).c_str());
          break;

        case 'b':
          printf("%s", name.getBloodType(rand()).c_str());
          break;

        case 'a':
          printf("%i", number.getAge(age));
          break;

        case 'o':
          printf("%i", number.getAgeGroup(age));
          break;

        case 'r':
          printf("%i", number.getReward(age));
          break;

        case 'y':
          printf("%i", number.getBirthYear(age));
          break;

        case 'd':
          printf("%i/%i/%i", 2020 - rand() % 3, number.getBirthMonth(age), number.getBirthDay(age));
          break;

        case 'n':
          printf("%i", (rand() % 20001 - 10000) * 1000);
          break;

        case 'q':
          printf("%1.2f", (double)(rand() % 100) / 100);
          break;

          //      case 'a': { printf("%s%s%s%d-%d", addr.getPrefecture(pref).c_str(), addr.getWard(pref, ward).c_str(), addr.getCity(pref, city).c_str(), rand()%100, rand()%100);
        }
      }
      printf("\n");

      //      printf("%s,%s,%s,090-%04d-%04d,%s%d-%d\n",
      //      name.LastName(last).c_str(), name.FirstName(first).c_str(), name.mailAddress(first, last).c_str(),
      //      rand()%1000, rand()%1000,addr.getAddress(rand()).c_str(), rand()%100, rand()%100);

      //      cout << name.LastName(last) << "," << name.FirstName(first) << "," << name.mailAddress(first, last) << ",";
      //      cout << "090-" << to_string(rand() % 1000) << "-" << to_string(rand() % 1000) << ",";
      //      cout << addr.getAddress(rand()) << to_string(rand() % 100) << "-" << to_string(rand() % 100) << endl;
    }
  }
  catch (string e)
  {
    cerr << e << endl;
    return 1;
  }
}
