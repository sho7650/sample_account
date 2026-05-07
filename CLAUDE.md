# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

`sample_account` is a C++17 command-line generator for synthetic Japanese personal-account records (name, address, age, mail, phone, etc.). Output is CSV on stdout, columns selected via short/long option flags. Data is sourced at runtime from CSV files in `data/`.

## Build & Run

```sh
make                    # debug build (default: -O0 -g)
make CXXFLAGS_OPT=-O2   # optimized build
make all                # clean rebuild
make clean
make test               # unit-test + snapshot-test
make unit-test          # C++ assertions in tests/test_repos.cpp
make snapshot-test      # CSV diff against tests/expected/

./sample_account --help
./sample_account [OPTIONS] [COUNT]   # COUNT defaults to 100
```

The binary must be run from the repo root because data file paths are relative (`data/sample_account.csv`, `data/prefectures.csv`, `data/address.csv`, `data/ages.csv`); running elsewhere triggers a `file not found` exception.

## Architecture

Three layers, dependencies flow downward only:

```
CLI Layer        — source/cli.cpp, source/main.cpp
                   parses argv, prints --help, runs the row loop
                       │
Generation Layer — source/fields.cpp, source/generators.cpp, source/rng.cpp
                   IField strategy + FieldRegistry,
                   PersonGenerator / AddressGenerator / AgeAndDateGenerator,
                   Rng façade
                       │
Data Layer       — source/csv_repos.cpp
                   CsvPersonRepo / CsvPrefectureRepo / CsvAgeRepo
                   implement IPersonRepo / IPrefectureRepo / IAgeRepo
```

### How a new column is added

1. Define a new `IField` subclass in `source/fields.cpp` — overrides `shortFlag()`, `longName()`, `description()`, `emit()`.
2. Register it in `buildDefaultRegistry()` (bottom of the same file).
3. That's it. The CLI parser, --help text, and getopt option strings all derive from the registry — no other edits needed.

### Per-row state

`RowContext` (in `include/field.h`) holds the random integers drawn once per row: `first`, `last`, `pref`, `ward`, `city`, `age`, plus the row index. Multiple fields can read the same context value to stay consistent (e.g., the same `first` index drives both first-name and email's local-part). `Rng::rollDate()` is also called once per row so the `--date` field's year/month/day are self-consistent.

### Repository contract

Repositories are read-only after construction. They expose `const std::vector<…>&` views — generators index into those vectors with modulo. Constructors do file I/O and throw `std::runtime_error` on missing files. To swap data sources (JSON, in-memory, etc.), implement the `I*Repo` interface in `include/repositories.h`.

## Determinism / Test Hooks

Two environment variables make output reproducible — used by `tests/snapshot.sh` and useful for any regression test:

- `SAMPLE_ACCOUNT_SEED` — pins the RNG seed (replaces `time(NULL)`-based seeding).
- `SAMPLE_ACCOUNT_NOW` — pins "current time" (Unix epoch seconds) used by `Rng::rollDate()` and `AgeAndDateGenerator::birthYear()`. Without this, dates drift between runs even with a fixed seed.

When regenerating expected snapshots, set both:

```sh
SAMPLE_ACCOUNT_SEED=42 SAMPLE_ACCOUNT_NOW=1700000000 ./sample_account ... > tests/expected/<name>.csv
```

## Data Files

CSVs are committed; do not regenerate casually.

- `data/sample_account.csv` — `last_kanji,last_kana,last_romaji,first_kanji,first_kana,first_romaji,gender,blood`
- `data/prefectures.csv` — `code,name,population` (47 rows; `zips` is computed at load time from `address.csv`)
- `data/address.csv` — `pref_code,prefecture,ward,city` (sorted by `pref_code`; the loader assumes contiguous grouping)
- `data/ages.csv` — `generation,population` (population uses thousand-separators; `CsvAgeRepo` strips non-digits before parsing)
- `tool/convert-address.sh` regenerates `address.csv` from raw Japan Post data (requires `nkf`).

## Notable Constraints / Gotchas

- `Account::LastName` and `Account::FirstName` no longer exist — they're `PersonGenerator::lastName/firstName` and still return `"<kanji>,<kana>"` (two CSV fields per call). This is intentional and documented in `include/generators.h`.
- The legacy `--telehpne` long-option typo is preserved as an alias for `--telephone` in `cli.cpp`'s longopts table; do not "fix" it without understanding which downstream scripts use it.
- The version macro lives at `include/sample_account.h` (`#define VERSION 0.4.7`) — bump it on release.
- `tests/test_repos.cpp` builds against the production source files (excluding `main.cpp`) via the Makefile's `LIB_SOURCES` filter. New test files should follow the same `tests/test_*.cpp` pattern and be added to the `TEST_BINS` list in the Makefile.

## Recent Commit Style

The project's earlier commit history uses terse imperative messages (`add date`, `update version`). Newer commits use conventional-commits format (`fix:`, `refactor:`). Either style is fine — match the surrounding context.
