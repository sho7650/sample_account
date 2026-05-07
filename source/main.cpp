#include <cstdio>
#include <exception>
#include <iostream>

#include "cli.h"
#include "csv_repos.h"
#include "field.h"
#include "field_registry.h"
#include "generators.h"
#include "rng.h"
#include "sample_account.h"

int main(int argc, char *argv[])
{
  try
  {
    FieldRegistry registry = buildDefaultRegistry();
    CliArgs args = parseArgs(argc, argv, registry);

    if (args.error)
    {
      std::fprintf(stderr, "%s: %s\n", argv[0], args.error_message.c_str());
      std::fprintf(stderr, "Try '%s --help' for usage.\n", argv[0]);
      return 2;
    }

    if (args.help)
    {
      printHelp(stdout, argv[0], registry);
      return 0;
    }

    CsvPersonRepo     person_repo;
    CsvPrefectureRepo prefecture_repo;
    CsvAgeRepo        age_repo;

    PersonGenerator     person(person_repo);
    AddressGenerator    address(prefecture_repo);
    AgeAndDateGenerator age_date(age_repo);
    Rng                 rng;
    FieldDeps           deps{person, address, age_date, rng};

    std::FILE *out = stdout;
    for (int i = 0; i < args.count; ++i)
    {
      RowContext ctx;
      ctx.row   = i;
      ctx.first = rng.next();
      ctx.last  = rng.next();
      ctx.pref  = address.weightedPrefectureIndex(rng.next());
      ctx.ward  = rng.next();
      ctx.city  = rng.next();
      ctx.age   = rng.next();
      rng.rollDate();

      for (size_t j = 0; j < args.selected_fields.size(); ++j)
      {
        if (j > 0)
          std::fputc(',', out);
        args.selected_fields[j]->emit(out, ctx, deps);
      }
      std::fputc('\n', out);
    }
  }
  catch (const std::exception &e)
  {
    std::fprintf(stderr, "%s: %s\n", argv[0], e.what());
    return 1;
  }

  return 0;
}
