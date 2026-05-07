#ifndef FIELD_H
#define FIELD_H

#include <cstdio>
#include <string>

#include "generators.h"
#include "rng.h"

// Per-row state shared by every field in a row. Each row resamples these
// once so that, for example, prefecture/ward/city stay consistent and the
// same `first` index drives both first-name and email.
struct RowContext
{
  int row;   // 0-based row index
  int first;
  int last;
  int pref;  // population-weighted prefecture index
  int ward;
  int city;
  int age;
};

// Bag-of-references handed to fields so they don't reach for globals.
struct FieldDeps
{
  const PersonGenerator     &person;
  const AddressGenerator    &address;
  const AgeAndDateGenerator &age_date;
  Rng                       &rng;
};

// Strategy interface for one CSV column. One concrete subclass per column.
// Adding a new column is one new subclass + one registration line.
class IField
{
public:
  virtual ~IField() = default;

  // Identifier used in getopt's short-option string and for fast dispatch
  // by the registry.
  virtual char shortFlag() const = 0;

  // Primary long-option name (without leading "--").
  virtual std::string longName() const = 0;

  // One-line description shown in --help output.
  virtual std::string description() const = 0;

  // Writes this column's value to `out`. Must NOT write a trailing comma.
  virtual void emit(std::FILE *out, const RowContext &ctx, FieldDeps &deps) const = 0;
};

#endif
