// Concrete IField subclasses, one per CSV column.
//
// Each field is small and self-contained. They share the same base interface
// so the registry can dispatch by short flag without a switch statement.
// Adding a new column = define a new subclass below and register it in
// buildDefaultRegistry() (at the bottom of this file).

#include <cstdio>
#include <cstdlib>
#include <memory>

#include "field.h"
#include "field_registry.h"

namespace {

// -----------------------------------------------------------------------------
// Identity / index
// -----------------------------------------------------------------------------

class IdField : public IField
{
public:
  char shortFlag() const override { return 'i'; }
  std::string longName() const override { return "id"; }
  std::string description() const override { return "sequential row id (1-based)"; }
  void emit(std::FILE *out, const RowContext &ctx, FieldDeps &) const override
  {
    std::fprintf(out, "%i", ctx.row + 1);
  }
};

// -----------------------------------------------------------------------------
// Person
// -----------------------------------------------------------------------------

class LastNameField : public IField
{
public:
  char shortFlag() const override { return 'l'; }
  std::string longName() const override { return "lastname"; }
  std::string description() const override { return "last name (kanji,kana — two CSV fields)"; }
  void emit(std::FILE *out, const RowContext &ctx, FieldDeps &deps) const override
  {
    std::fprintf(out, "%s", deps.person.lastName(ctx.last).c_str());
  }
};

class FirstNameField : public IField
{
public:
  char shortFlag() const override { return 'f'; }
  std::string longName() const override { return "firstname"; }
  std::string description() const override { return "first name (kanji,kana — two CSV fields)"; }
  void emit(std::FILE *out, const RowContext &ctx, FieldDeps &deps) const override
  {
    std::fprintf(out, "%s", deps.person.firstName(ctx.first).c_str());
  }
};

class MailField : public IField
{
public:
  char shortFlag() const override { return 'm'; }
  std::string longName() const override { return "mail"; }
  std::string description() const override { return "email address (firstname_lastname@example.com)"; }
  void emit(std::FILE *out, const RowContext &ctx, FieldDeps &deps) const override
  {
    std::fprintf(out, "%s", deps.person.mailAddress(ctx.first, ctx.last).c_str());
  }
};

class GenderField : public IField
{
public:
  char shortFlag() const override { return 'g'; }
  std::string longName() const override { return "gender"; }
  std::string description() const override { return "gender (男 / 女)"; }
  void emit(std::FILE *out, const RowContext &ctx, FieldDeps &deps) const override
  {
    std::fprintf(out, "%s", deps.person.gender(ctx.first).c_str());
  }
};

class BloodField : public IField
{
public:
  char shortFlag() const override { return 'b'; }
  std::string longName() const override { return "blood"; }
  std::string description() const override { return "ABO blood type"; }
  void emit(std::FILE *out, const RowContext &, FieldDeps &deps) const override
  {
    std::fprintf(out, "%s", deps.person.bloodType(deps.rng.next()).c_str());
  }
};

class TelephoneField : public IField
{
public:
  char shortFlag() const override { return 't'; }
  std::string longName() const override { return "telephone"; }
  std::string description() const override { return "phone number (090-XXXX-XXXX)"; }
  void emit(std::FILE *out, const RowContext &, FieldDeps &deps) const override
  {
    std::fprintf(out, "090-%04d-%04d", deps.rng.next() % 1000, deps.rng.next() % 1000);
  }
};

// -----------------------------------------------------------------------------
// Address
// -----------------------------------------------------------------------------

class PrefectureField : public IField
{
public:
  char shortFlag() const override { return 'p'; }
  std::string longName() const override { return "prefecture"; }
  std::string description() const override { return "prefecture name (population-weighted)"; }
  void emit(std::FILE *out, const RowContext &ctx, FieldDeps &deps) const override
  {
    std::fprintf(out, "%s", deps.address.prefectureName(ctx.pref).c_str());
  }
};

class WardField : public IField
{
public:
  char shortFlag() const override { return 'w'; }
  std::string longName() const override { return "ward"; }
  std::string description() const override { return "ward / municipality within the prefecture"; }
  void emit(std::FILE *out, const RowContext &ctx, FieldDeps &deps) const override
  {
    std::fprintf(out, "%s", deps.address.ward(ctx.pref, ctx.ward).c_str());
  }
};

class CityField : public IField
{
public:
  char shortFlag() const override { return 'c'; }
  std::string longName() const override { return "city"; }
  std::string description() const override { return "city / district within the ward"; }
  void emit(std::FILE *out, const RowContext &ctx, FieldDeps &deps) const override
  {
    std::fprintf(out, "%s", deps.address.city(ctx.pref, ctx.city).c_str());
  }
};

// -----------------------------------------------------------------------------
// Age / date / numeric
// -----------------------------------------------------------------------------

class AgeField : public IField
{
public:
  char shortFlag() const override { return 'a'; }
  std::string longName() const override { return "age"; }
  std::string description() const override { return "age in years (population-weighted)"; }
  void emit(std::FILE *out, const RowContext &ctx, FieldDeps &deps) const override
  {
    std::fprintf(out, "%i", deps.age_date.age(ctx.age));
  }
};

class AgeGroupField : public IField
{
public:
  char shortFlag() const override { return 'o'; }
  std::string longName() const override { return "agegroup"; }
  std::string description() const override { return "age group rounded down to the decade"; }
  void emit(std::FILE *out, const RowContext &ctx, FieldDeps &deps) const override
  {
    std::fprintf(out, "%i", deps.age_date.ageGroup(ctx.age));
  }
};

class BirthYearField : public IField
{
public:
  char shortFlag() const override { return 'y'; }
  std::string longName() const override { return "birthyear"; }
  std::string description() const override { return "birth year derived from age"; }
  void emit(std::FILE *out, const RowContext &ctx, FieldDeps &deps) const override
  {
    std::fprintf(out, "%i", deps.age_date.birthYear(ctx.age));
  }
};

class RewardField : public IField
{
public:
  char shortFlag() const override { return 'r'; }
  std::string longName() const override { return "reward"; }
  std::string description() const override { return "annual income-like figure derived from age group"; }
  void emit(std::FILE *out, const RowContext &ctx, FieldDeps &deps) const override
  {
    std::fprintf(out, "%i", deps.age_date.reward(ctx.age, deps.rng));
  }
};

class DateField : public IField
{
public:
  char shortFlag() const override { return 'd'; }
  std::string longName() const override { return "date"; }
  std::string description() const override { return "random valid date (YYYY/M/D)"; }
  void emit(std::FILE *out, const RowContext &, FieldDeps &deps) const override
  {
    std::fprintf(out, "%i/%i/%i", deps.rng.year(), deps.rng.month(), deps.rng.day());
  }
};

class RandomIntField : public IField
{
public:
  char shortFlag() const override { return 'n'; }
  std::string longName() const override { return "random"; }
  std::string description() const override { return "random signed integer in ±10,000,000"; }
  void emit(std::FILE *out, const RowContext &, FieldDeps &deps) const override
  {
    std::fprintf(out, "%i", (deps.rng.next() % 20001 - 10000) * 1000);
  }
};

class QuotientField : public IField
{
public:
  char shortFlag() const override { return 'q'; }
  std::string longName() const override { return "quotient"; }
  std::string description() const override { return "random fraction in [0.00, 0.99]"; }
  void emit(std::FILE *out, const RowContext &, FieldDeps &deps) const override
  {
    std::fprintf(out, "%1.2f", static_cast<double>(deps.rng.next() % 100) / 100);
  }
};

} // namespace

// -----------------------------------------------------------------------------
// FieldRegistry
// -----------------------------------------------------------------------------

void FieldRegistry::add(std::unique_ptr<IField> f)
{
  fields_.push_back(std::move(f));
}

const IField *FieldRegistry::find(char short_flag) const
{
  for (const auto &f : fields_)
    if (f->shortFlag() == short_flag)
      return f.get();
  return nullptr;
}

std::string FieldRegistry::shortOptString() const
{
  std::string s;
  s.reserve(fields_.size());
  for (const auto &f : fields_)
    s.push_back(f->shortFlag());
  return s;
}

FieldRegistry buildDefaultRegistry()
{
  FieldRegistry reg;
  reg.add(std::make_unique<IdField>());
  reg.add(std::make_unique<LastNameField>());
  reg.add(std::make_unique<FirstNameField>());
  reg.add(std::make_unique<MailField>());
  reg.add(std::make_unique<TelephoneField>());
  reg.add(std::make_unique<PrefectureField>());
  reg.add(std::make_unique<WardField>());
  reg.add(std::make_unique<CityField>());
  reg.add(std::make_unique<GenderField>());
  reg.add(std::make_unique<BloodField>());
  reg.add(std::make_unique<AgeField>());
  reg.add(std::make_unique<AgeGroupField>());
  reg.add(std::make_unique<BirthYearField>());
  reg.add(std::make_unique<RewardField>());
  reg.add(std::make_unique<DateField>());
  reg.add(std::make_unique<RandomIntField>());
  reg.add(std::make_unique<QuotientField>());
  return reg;
}
