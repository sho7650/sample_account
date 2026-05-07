#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <getopt.h>
#include <iostream>
#include <string>

#include "sample_account.h"
#include "accounts.h"
#include "addresses.h"
#include "random.h"

using namespace std;

namespace {

struct RowContext
{
  int row;   // 0-based row index
  int first; // rand() reused across name/mail
  int last;  // rand() reused across name/mail
  int pref;  // population-weighted prefecture index
  int ward;  // rand() shared by prefecture's ward selection
  int city;  // rand() shared by prefecture's city selection
  int age;   // rand() reused across age/agegroup/reward/birthyear
};

void emitColumn(char opt, const RowContext &ctx,
                Account &name, Prefecture &addr, Random &number)
{
  switch (opt)
  {
  case 'i':
    printf("%i", ctx.row + 1);
    break;
  case 'l':
    printf("%s", name.LastName(ctx.last).c_str());
    break;
  case 'f':
    printf("%s", name.FirstName(ctx.first).c_str());
    break;
  case 'm':
    printf("%s", name.mailAddress(ctx.first, ctx.last).c_str());
    break;
  case 't':
    printf("090-%04d-%04d", rand() % 1000, rand() % 1000);
    break;
  case 'p':
    printf("%s", addr.getPrefecture(ctx.pref).c_str());
    break;
  case 'w':
    printf("%s", addr.getWard(ctx.pref, ctx.ward).c_str());
    break;
  case 'c':
    printf("%s", addr.getCity(ctx.pref, ctx.city).c_str());
    break;
  case 'g':
    printf("%s", name.getGender(ctx.first).c_str());
    break;
  case 'b':
    printf("%s", name.getBloodType(rand()).c_str());
    break;
  case 'a':
    printf("%i", number.getAge(ctx.age));
    break;
  case 'o':
    printf("%i", number.getAgeGroup(ctx.age));
    break;
  case 'r':
    printf("%i", number.getReward(ctx.age));
    break;
  case 'y':
    printf("%i", number.getBirthYear(ctx.age));
    break;
  case 'd':
    printf("%i/%i/%i", number.getRandomYear(), number.getRandomMonth(), number.getRandomDay());
    break;
  case 'n':
    printf("%i", (rand() % 20001 - 10000) * 1000);
    break;
  case 'q':
    printf("%1.2f", static_cast<double>(rand() % 100) / 100);
    break;
  }
}

} // namespace

int main(int argc, char *argv[])
{
  int max = MAXDEF;
  int opts[OPTIONS], max_opts = 0;
  srand(static_cast<unsigned>(time(NULL)));

  struct option longopts[] = {
      {"id", no_argument, NULL, 'i'},
      {"lastname", no_argument, NULL, 'l'},
      {"firstname", no_argument, NULL, 'f'},
      {"mail", no_argument, NULL, 'm'},
      {"age", no_argument, NULL, 'a'},
      {"agegroup", no_argument, NULL, 'o'},
      {"birthyear", no_argument, NULL, 'y'},
      {"telephone", no_argument, NULL, 't'},
      {"telehpne", no_argument, NULL, 't'}, // legacy typo, kept for backward compatibility
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
    if (max_opts < OPTIONS)
      opts[max_opts++] = opt;
    // Silently drop overflow flags rather than overrunning the stack buffer.
  }

  if (max_opts == 0)
  {
    opts[max_opts++] = 'i';
  }

  if (optind < argc)
  {
    int parsed = atoi(argv[optind]);
    if (parsed > 0)
      max = parsed;
  }

  try
  {
    Account name;
    Prefecture addr;
    Random number;

    for (int i = 0; i < max; i++)
    {
      RowContext ctx;
      ctx.row = i;
      ctx.first = rand();
      ctx.last = rand();
      ctx.pref = addr.setPrefNumber(rand());
      ctx.ward = rand();
      ctx.city = rand();
      ctx.age = rand();
      number.setTime();

      for (int j = 0; j < max_opts; j++)
      {
        if (j > 0)
          printf(",");
        emitColumn(static_cast<char>(opts[j]), ctx, name, addr, number);
      }
      printf("\n");
    }
  }
  catch (string e)
  {
    cerr << e << endl;
    return 1;
  }
}
