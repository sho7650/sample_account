#ifndef GENERATORS_H
#define GENERATORS_H

#include <string>

#include "repositories.h"
#include "rng.h"

// Higher-level domain operations that fields invoke. These wrap repository
// records with the lookup logic needed for each column. They replace the
// old Account / Prefecture / Random classes and have no file-loading
// responsibility — repositories are injected by the caller.

class PersonGenerator
{
  const IPersonRepo &repo_;

public:
  explicit PersonGenerator(const IPersonRepo &repo) : repo_(repo) {}

  // Returns "<kanji>,<kana>" — TWO comma-separated CSV fields.
  std::string lastName(int n) const;
  std::string firstName(int n) const;

  std::string mailAddress(int first, int last) const;
  std::string gender(int n) const;
  std::string bloodType(int n) const;
};

class AddressGenerator
{
  const IPrefectureRepo &repo_;

public:
  explicit AddressGenerator(const IPrefectureRepo &repo) : repo_(repo) {}

  // Picks a prefecture index weighted by population.
  int weightedPrefectureIndex(int n) const;

  std::string prefectureName(int pref_idx) const;
  std::string ward(int pref_idx, int n) const;
  std::string city(int pref_idx, int n) const;

private:
  // Translates (prefecture index, n) into a global address-row index.
  int addressIndex(int pref_idx, int n) const;
};

class AgeAndDateGenerator
{
  const IAgeRepo &repo_;

public:
  explicit AgeAndDateGenerator(const IAgeRepo &repo) : repo_(repo) {}

  int age(int n) const;
  int ageGroup(int n) const;
  int birthYear(int n) const;
  int reward(int n, Rng &rng) const;
};

#endif
