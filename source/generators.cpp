#include <cstdlib>

#include "generators.h"

#define MAIL_DOMAIN "example.com"

namespace {

size_t findAgeBucket(const std::vector<AgeBucket> &buckets, int total)
{
  size_t i = 0;
  while (i + 1 < buckets.size() && buckets[i + 1].start <= total)
    ++i;
  return i;
}

} // namespace

// -----------------------------------------------------------------------------
// PersonGenerator
// -----------------------------------------------------------------------------

std::string PersonGenerator::lastName(int n) const
{
  const auto &recs = repo_.records();
  const auto &r = recs[n % recs.size()];
  return r.last_kanji + "," + r.last_kana;
}

std::string PersonGenerator::firstName(int n) const
{
  const auto &recs = repo_.records();
  const auto &r = recs[n % recs.size()];
  return r.first_kanji + "," + r.first_kana;
}

std::string PersonGenerator::mailAddress(int first, int last) const
{
  const auto &recs = repo_.records();
  return recs[first % recs.size()].first_name + "_"
       + recs[last  % recs.size()].last_name  + "@" + MAIL_DOMAIN;
}

std::string PersonGenerator::gender(int n) const
{
  const auto &recs = repo_.records();
  return recs[n % recs.size()].gender;
}

std::string PersonGenerator::bloodType(int n) const
{
  const auto &recs = repo_.records();
  return recs[n % recs.size()].blood_type;
}

// -----------------------------------------------------------------------------
// AddressGenerator
// -----------------------------------------------------------------------------

int AddressGenerator::weightedPrefectureIndex(int n) const
{
  const auto &prefs = repo_.prefectures();
  int target = n % repo_.totalPopulation();
  int running = 0;
  for (size_t i = 0; i < prefs.size(); ++i)
  {
    running += prefs[i].population;
    if (target < running)
      return static_cast<int>(i);
  }
  return static_cast<int>(prefs.size() - 1);
}

std::string AddressGenerator::prefectureName(int pref_idx) const
{
  const auto &prefs = repo_.prefectures();
  if (pref_idx < 0 || pref_idx >= static_cast<int>(prefs.size()))
    pref_idx = 0;
  return prefs[pref_idx].name;
}

int AddressGenerator::addressIndex(int pref_idx, int n) const
{
  const auto &prefs = repo_.prefectures();
  const int total_addresses = static_cast<int>(repo_.addresses().size());

  if (pref_idx < 0 || pref_idx >= static_cast<int>(prefs.size()))
    pref_idx = 0;

  int offset = 0;
  for (int i = 0; i < pref_idx; ++i)
    offset += prefs[i].zips;

  int zips = prefs[pref_idx].zips;
  if (zips <= 0)
    return offset < total_addresses ? offset : 0;

  if (n < 0)
    n = -n;
  return (n % zips) + offset;
}

std::string AddressGenerator::ward(int pref_idx, int n) const
{
  return repo_.addresses()[addressIndex(pref_idx, n)].ward;
}

std::string AddressGenerator::city(int pref_idx, int n) const
{
  return repo_.addresses()[addressIndex(pref_idx, n)].city;
}

// -----------------------------------------------------------------------------
// AgeAndDateGenerator
// -----------------------------------------------------------------------------

int AgeAndDateGenerator::age(int n) const
{
  const auto &buckets = repo_.buckets();
  int total = n % repo_.totalAge();
  size_t i = findAgeBucket(buckets, total);
  return buckets[i].generation + n % 5;
}

int AgeAndDateGenerator::ageGroup(int n) const
{
  const auto &buckets = repo_.buckets();
  int total = n % repo_.totalAge();
  size_t i = findAgeBucket(buckets, total);
  return (buckets[i].generation / 10) * 10;
}

int AgeAndDateGenerator::birthYear(int n) const
{
  std::time_t now = Rng::currentTime();
  const std::tm *lt = std::localtime(&now);
  return lt->tm_year + 1900 - age(n);
}

int AgeAndDateGenerator::reward(int n, Rng &rng) const
{
  return (50 - std::abs(ageGroup(n) - 50) + (rng.next() % 5))
       * (rng.next() % 3 + 1) * 100000;
}
