// Unit tests for the repository layer. Built as a separate binary; assertions
// abort on failure with a non-zero exit code.

#include <cassert>
#include <cstdio>

#include "csv_repos.h"

static int run_count = 0;
static int fail_count = 0;

#define TEST(name)                                                    \
  do {                                                                \
    fprintf(stderr, "RUN  %s\n", #name);                              \
    ++run_count;                                                      \
    try { name(); fprintf(stderr, "PASS %s\n", #name); }              \
    catch (const std::exception &e) {                                 \
      ++fail_count;                                                   \
      fprintf(stderr, "FAIL %s: %s\n", #name, e.what());              \
    }                                                                 \
  } while (0)

void person_repo_loads_records()
{
  CsvPersonRepo repo;
  const auto &recs = repo.records();
  assert(!recs.empty());
  // Sanity: all records have non-empty kanji last name.
  for (const auto &r : recs)
    assert(!r.last_kanji.empty());
}

void prefecture_repo_loads_47_prefectures()
{
  CsvPrefectureRepo repo;
  assert(repo.prefectures().size() == 47);
  assert(repo.totalPopulation() > 100'000'000); // Japan is ~125M
  assert(!repo.addresses().empty());
}

void prefecture_repo_assigns_zips_to_each_prefecture()
{
  CsvPrefectureRepo repo;
  int total_zips = 0;
  for (const auto &p : repo.prefectures())
  {
    assert(p.zips >= 0);
    total_zips += p.zips;
  }
  // Sum of per-prefecture zip counts must equal total addresses loaded.
  assert(static_cast<size_t>(total_zips) == repo.addresses().size());
}

void age_repo_strips_thousand_separators()
{
  CsvAgeRepo repo;
  const auto &buckets = repo.buckets();
  assert(!buckets.empty());
  // First bucket in data/ages.csv is "0,4,987,706" => population must be
  // millions, not the single digit 4 that the old parser produced.
  assert(buckets[0].population > 1'000'000);
  // Total population should roughly match Japan's adult+child population.
  assert(repo.totalAge() > 100'000'000);
}

void person_repo_throws_on_missing_file()
{
  bool threw = false;
  try {
    CsvPersonRepo repo("data/does-not-exist.csv");
  } catch (const std::runtime_error &) {
    threw = true;
  }
  assert(threw);
}

int main()
{
  TEST(person_repo_loads_records);
  TEST(prefecture_repo_loads_47_prefectures);
  TEST(prefecture_repo_assigns_zips_to_each_prefecture);
  TEST(age_repo_strips_thousand_separators);
  TEST(person_repo_throws_on_missing_file);

  fprintf(stderr, "\n%d run, %d failed\n", run_count, fail_count);
  return fail_count == 0 ? 0 : 1;
}
