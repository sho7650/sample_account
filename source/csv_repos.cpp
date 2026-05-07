#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "csv_repos.h"

namespace {

std::ifstream openOrThrow(const std::string &path)
{
  std::ifstream ifs(path);
  if (!ifs)
    throw std::runtime_error("file not found: " + path);
  return ifs;
}

// data/ages.csv uses thousand-separators (e.g. "4,987,706"); strip non-digits
// before parsing.
int parseDigits(const std::string &s)
{
  std::string digits;
  digits.reserve(s.size());
  for (char c : s)
  {
    if (c >= '0' && c <= '9')
      digits.push_back(c);
  }
  return digits.empty() ? 0 : std::stoi(digits);
}

} // namespace

// -----------------------------------------------------------------------------
// CsvPersonRepo
// -----------------------------------------------------------------------------

CsvPersonRepo::CsvPersonRepo(const std::string &path)
{
  auto ifs = openOrThrow(path);

  std::string line;
  while (std::getline(ifs, line))
  {
    PersonRecord rec;
    std::istringstream stream(line);
    std::getline(stream, rec.last_kanji,  ',');
    std::getline(stream, rec.last_kana,   ',');
    std::getline(stream, rec.last_name,   ',');
    std::getline(stream, rec.first_kanji, ',');
    std::getline(stream, rec.first_kana,  ',');
    std::getline(stream, rec.first_name,  ',');
    std::getline(stream, rec.gender,      ',');
    std::getline(stream, rec.blood_type);
    data_.push_back(std::move(rec));
  }
}

// -----------------------------------------------------------------------------
// CsvPrefectureRepo
// -----------------------------------------------------------------------------

CsvPrefectureRepo::CsvPrefectureRepo(const std::string &prefecture_path,
                                     const std::string &address_path)
{
  // Pass 1: prefectures.
  {
    auto ifs = openOrThrow(prefecture_path);
    std::string line;
    while (std::getline(ifs, line))
    {
      PrefectureRecord rec{};
      std::istringstream stream(line);
      std::string token;
      std::getline(stream, token, ',');
      rec.number = std::stoi(token);
      std::getline(stream, rec.name, ',');
      std::getline(stream, token);
      rec.population = std::stoi(token);
      total_population_ += rec.population;
      prefectures_.push_back(std::move(rec));
    }
  }

  // Pass 2: addresses, computing per-prefecture address counts as we go.
  // Assumption (matches existing data file): addresses are sorted by prefecture
  // number, contiguous within each prefecture.
  auto ifs = openOrThrow(address_path);
  int zip_count    = 0;
  int current_pref = 0; // sentinel: no prefecture seen yet
  const int n_prefs = static_cast<int>(prefectures_.size());

  std::string line;
  while (std::getline(ifs, line))
  {
    AddressRecord rec{};
    std::istringstream stream(line);
    std::string token;
    std::getline(stream, token, ',');
    rec.number = std::stoi(token);
    std::getline(stream, rec.prefecture, ',');
    std::getline(stream, rec.ward,       ',');
    std::getline(stream, rec.city);
    addresses_.push_back(std::move(rec));

    if (current_pref != addresses_.back().number)
    {
      // Boundary: flush prior prefecture's count, then start a new one.
      if (current_pref >= 1 && current_pref <= n_prefs)
        prefectures_[current_pref - 1].zips = zip_count;
      current_pref = addresses_.back().number;
      zip_count    = 1; // count the current row in the new prefecture
    }
    else
    {
      ++zip_count;
    }
  }
  if (current_pref >= 1 && current_pref <= n_prefs)
    prefectures_[current_pref - 1].zips = zip_count;
}

// -----------------------------------------------------------------------------
// CsvAgeRepo
// -----------------------------------------------------------------------------

CsvAgeRepo::CsvAgeRepo(const std::string &path)
{
  auto ifs = openOrThrow(path);

  std::string line;
  while (std::getline(ifs, line))
  {
    AgeBucket b{};
    std::istringstream stream(line);
    std::string token;
    std::getline(stream, token, ',');
    b.generation = std::stoi(token);
    std::getline(stream, token);
    b.population = parseDigits(token); // strips thousand-separators
    b.start      = total_age_;
    total_age_  += b.population;
    data_.push_back(std::move(b));
  }
}
