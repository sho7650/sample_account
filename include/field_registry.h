#ifndef FIELD_REGISTRY_H
#define FIELD_REGISTRY_H

#include <memory>
#include <string>
#include <vector>

#include "field.h"

// Owns every IField in the program. Provides:
//   - dispatch by short flag (used per row)
//   - the getopt short-option string and longopts table (used by the CLI)
//   - iteration for --help output (single source of truth)
class FieldRegistry
{
  std::vector<std::unique_ptr<IField>> fields_;

public:
  void add(std::unique_ptr<IField> field);

  // Returns nullptr if no field is registered for the given short flag.
  const IField *find(char short_flag) const;

  const std::vector<std::unique_ptr<IField>> &all() const { return fields_; }

  // Returns the concatenation of every registered short flag — suitable
  // for getopt's optstring argument.
  std::string shortOptString() const;
};

// Builds the registry containing every field this binary supports, in the
// order they should appear in --help output.
FieldRegistry buildDefaultRegistry();

#endif
