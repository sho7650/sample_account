#ifndef CLI_H
#define CLI_H

#include <cstdio>
#include <string>
#include <vector>

#include "field_registry.h"

#define DEFAULT_ROW_COUNT 100

struct CliArgs
{
  std::vector<const IField *> selected_fields;
  int  count    = DEFAULT_ROW_COUNT;
  bool help     = false;
  bool error    = false;
  std::string error_message;
};

// Parses argv against the registry. Unknown flags set args.error = true and
// fill args.error_message. --help / -h set args.help and short-circuit.
// When no field flags are given, defaults to a single IdField (preserves
// historical behavior).
CliArgs parseArgs(int argc, char *argv[], const FieldRegistry &registry);

// Prints usage / column reference table to `out`. Pulls field metadata
// directly from the registry so help can never drift from real flags.
void printHelp(std::FILE *out, const char *prog_name, const FieldRegistry &registry);

#endif
