#ifndef CSV_REPOS_H
#define CSV_REPOS_H

#include <string>
#include <vector>

#include "repositories.h"

// Default file paths used when no explicit path is supplied. These are
// relative to the current working directory; callers must run the binary
// from the project root.
#define DEFAULT_PERSON_CSV     "data/sample_account.csv"
#define DEFAULT_PREFECTURE_CSV "data/prefectures.csv"
#define DEFAULT_ADDRESS_CSV    "data/address.csv"
#define DEFAULT_AGE_CSV        "data/ages.csv"

class CsvPersonRepo : public IPersonRepo
{
  std::vector<PersonRecord> data_;

public:
  explicit CsvPersonRepo(const std::string &path = DEFAULT_PERSON_CSV);
  const std::vector<PersonRecord> &records() const override { return data_; }
};

class CsvPrefectureRepo : public IPrefectureRepo
{
  std::vector<PrefectureRecord> prefectures_;
  std::vector<AddressRecord>    addresses_;
  int                           total_population_ = 0;

public:
  CsvPrefectureRepo(const std::string &prefecture_path = DEFAULT_PREFECTURE_CSV,
                    const std::string &address_path    = DEFAULT_ADDRESS_CSV);
  const std::vector<PrefectureRecord> &prefectures() const override { return prefectures_; }
  const std::vector<AddressRecord>    &addresses()   const override { return addresses_; }
  int totalPopulation() const override { return total_population_; }
};

class CsvAgeRepo : public IAgeRepo
{
  std::vector<AgeBucket> data_;
  int                    total_age_ = 0;

public:
  explicit CsvAgeRepo(const std::string &path = DEFAULT_AGE_CSV);
  const std::vector<AgeBucket> &buckets() const override { return data_; }
  int totalAge() const override { return total_age_; }
};

#endif
