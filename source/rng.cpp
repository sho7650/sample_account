#include <cstdint>
#include <cstdlib>

#include "rng.h"

Rng::Rng()
{
  if (const char *seed_env = std::getenv("SAMPLE_ACCOUNT_SEED"))
    std::srand(static_cast<unsigned>(std::atol(seed_env)));
  else
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

int Rng::next()
{
  return std::rand();
}

void Rng::rollDate()
{
  std::time_t reference = currentTime();
  // rand() returns at most ~2^31; combine two draws to span time_t's range.
  uint64_t r = (static_cast<uint64_t>(std::rand()) << 32)
             ^ static_cast<uint32_t>(std::rand());
  now_ = static_cast<std::time_t>(r % static_cast<uint64_t>(reference));
}

int Rng::year() const
{
  const std::tm *lt = std::localtime(&now_);
  return lt->tm_year + 1900;
}

int Rng::month() const
{
  const std::tm *lt = std::localtime(&now_);
  return lt->tm_mon + 1;
}

int Rng::day() const
{
  const std::tm *lt = std::localtime(&now_);
  return lt->tm_mday;
}

std::time_t Rng::currentTime()
{
  if (const char *now_env = std::getenv("SAMPLE_ACCOUNT_NOW"))
    return static_cast<std::time_t>(std::atoll(now_env));
  return std::time(nullptr);
}
