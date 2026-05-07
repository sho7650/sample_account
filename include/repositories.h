#ifndef REPOSITORIES_H
#define REPOSITORIES_H

#include <string>
#include <vector>

// Plain data records loaded from disk. Repositories are read-only after
// construction; consumers receive const-references to the underlying vectors
// and look up records by index modulo the size.

struct PersonRecord
{
  std::string last_kanji;
  std::string last_kana;
  std::string last_name;
  std::string first_kanji;
  std::string first_kana;
  std::string first_name;
  std::string gender;
  std::string blood_type;
};

struct PrefectureRecord
{
  int         number;
  std::string name;
  int         population;
  int         zips; // computed at load time from the address file
};

struct AddressRecord
{
  int         number;
  std::string prefecture;
  std::string ward;
  std::string city;
};

struct AgeBucket
{
  int generation;
  int population;
  int start; // running sum used for weighted lookup
};

class IPersonRepo
{
public:
  virtual ~IPersonRepo()                                       = default;
  virtual const std::vector<PersonRecord> &records() const     = 0;
};

class IPrefectureRepo
{
public:
  virtual ~IPrefectureRepo()                                            = default;
  virtual const std::vector<PrefectureRecord> &prefectures() const      = 0;
  virtual const std::vector<AddressRecord> &addresses() const           = 0;
  virtual int totalPopulation() const                                   = 0;
};

class IAgeRepo
{
public:
  virtual ~IAgeRepo()                                = default;
  virtual const std::vector<AgeBucket> &buckets() const = 0;
  virtual int totalAge() const                          = 0;
};

#endif
