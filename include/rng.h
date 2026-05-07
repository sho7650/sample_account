#ifndef RNG_H
#define RNG_H

#include <ctime>

// Thin façade around C's rand()/srand() so callers don't reach for globals
// directly. Keeps a per-row "now" snapshot for deterministic date columns.
class Rng
{
  std::time_t now_ = 0;

public:
  // Reads SAMPLE_ACCOUNT_SEED for a fixed seed (used by tests); otherwise
  // seeds from the current time.
  Rng();

  // Returns a non-negative pseudo-random int from the underlying generator.
  int next();

  // Picks a random time_t in [0, currentTime()) and stores it. The three
  // year/month/day getters return components of this snapshot, so a single
  // call here yields a self-consistent random date.
  void rollDate();

  int year() const;
  int month() const;
  int day() const;

  // Returns SAMPLE_ACCOUNT_NOW (Unix epoch seconds) when set, else
  // std::time(nullptr). Exposed because birth-year math also depends on
  // a "current year" reference.
  static std::time_t currentTime();
};

#endif
