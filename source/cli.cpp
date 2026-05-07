#include <cstdlib>
#include <cstring>
#include <getopt.h>
#include <vector>

#include "cli.h"

namespace {

// Builds the longopts table for getopt_long from registered fields, plus the
// fixed entries (help and the legacy --telehpne typo alias).
struct LongOpts
{
  std::vector<option> table;            // Owns option entries.
  std::vector<std::string> name_storage; // Owns name strings (option.name is char*).
};

LongOpts buildLongOpts(const FieldRegistry &registry)
{
  LongOpts lo;
  lo.name_storage.reserve(registry.all().size() + 4);

  auto add = [&](const std::string &name, int val) {
    lo.name_storage.push_back(name);
    option opt{};
    opt.name    = lo.name_storage.back().c_str();
    opt.has_arg = no_argument;
    opt.flag    = nullptr;
    opt.val     = val;
    lo.table.push_back(opt);
  };

  for (const auto &f : registry.all())
    add(f->longName(), f->shortFlag());

  // Backward-compat aliases — preserved so existing scripts keep working.
  add("telehpne", 't'); // legacy typo for --telephone
  add("help",     'h');

  // Sentinel.
  option zero{};
  lo.table.push_back(zero);

  // option.name pointers can dangle if name_storage reallocates; reserve()
  // above ensures we never reallocate, so the pointers remain valid.
  return lo;
}

} // namespace

CliArgs parseArgs(int argc, char *argv[], const FieldRegistry &registry)
{
  CliArgs args;

  std::string optstring = registry.shortOptString() + "h";
  LongOpts longopts = buildLongOpts(registry);

  // Reset getopt's internal state in case parseArgs is called multiple times.
  optind = 1;
  opterr = 0; // suppress getopt's automatic error messages; we craft our own.

  int opt;
  int longindex = 0;
  while ((opt = getopt_long(argc, argv, optstring.c_str(),
                            longopts.table.data(), &longindex)) != -1)
  {
    if (opt == 'h')
    {
      args.help = true;
      return args;
    }
    if (opt == '?')
    {
      args.error = true;
      args.error_message = "unrecognized option";
      return args;
    }

    const IField *f = registry.find(static_cast<char>(opt));
    if (!f)
    {
      args.error = true;
      args.error_message = std::string("internal: no field for flag '") +
                           static_cast<char>(opt) + "'";
      return args;
    }
    args.selected_fields.push_back(f);
  }

  if (args.selected_fields.empty())
  {
    if (const IField *id = registry.find('i'))
      args.selected_fields.push_back(id);
  }

  if (optind < argc)
  {
    int parsed = std::atoi(argv[optind]);
    if (parsed > 0)
      args.count = parsed;
  }

  return args;
}

void printHelp(std::FILE *out, const char *prog_name, const FieldRegistry &registry)
{
  std::fprintf(out,
    "Usage: %s [OPTIONS] [COUNT]\n"
    "\n"
    "Generate synthetic Japanese sample-account records as CSV on stdout.\n"
    "\n"
    "Output columns are emitted in the order their flags appear on the\n"
    "command line. With no flags, a single id column is emitted.\n"
    "COUNT defaults to %d.\n"
    "\n"
    "Options:\n"
    "  -h, --help            show this help and exit\n",
    prog_name, DEFAULT_ROW_COUNT);

  for (const auto &f : registry.all())
  {
    std::fprintf(out, "  -%c, --%-13s %s\n",
                 f->shortFlag(),
                 f->longName().c_str(),
                 f->description().c_str());
  }

  std::fprintf(out,
    "\n"
    "Aliases:\n"
    "  --telehpne            legacy alias for --telephone\n"
    "\n"
    "Environment variables (mainly for testing):\n"
    "  SAMPLE_ACCOUNT_SEED   pin RNG seed for reproducible output\n"
    "  SAMPLE_ACCOUNT_NOW    pin \"current time\" (Unix epoch seconds)\n"
    "\n"
    "Examples:\n"
    "  %s -ilfm 10                    # id, last/first name (kanji,kana), email\n"
    "  %s --age --prefecture 5        # age and prefecture for 5 rows\n"
    "  %s -ilfmpwc 100 > out.csv      # full record with address columns\n",
    prog_name, prog_name, prog_name);
}
