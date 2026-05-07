# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

`sample_account` is a C++ command-line generator for synthetic Japanese personal-account records (name, address, age, mail, phone, etc.). Output is CSV on stdout, columns selected via short/long option flags. Data is sourced at runtime from CSV files in `data/`.

## Build & Run

```sh
make            # build ./sample_account (drops .o/.d into ./obj)
make all        # clean rebuild
make clean      # remove obj/, deps, and binary
./sample_account [OPTIONS] [COUNT]   # COUNT defaults to MAXDEF=100
```

The Makefile compiles every `source/*.cpp` into `obj/`, links to `./sample_account`. Headers come from `./include` and `/usr/local/opt/openssl/include` (Homebrew OpenSSL — only relevant if `hash.h`/SHA features are re-enabled; currently commented out in `source/sample_account.cpp`). There are no tests, linters, or CI configured.

The binary must be run from the repo root because data file paths are relative (`data/sample_account.csv`, `./data/prefectures.csv`, `./data/address.csv`, `data/ages.csv`); running elsewhere triggers the `Exception: file not found` throw in the file readers.

### CLI flags

`getopt_long` short/long options in `source/sample_account.cpp:24` map directly to switch cases that print one column each, comma-separated, in the order they appear on the command line. With no flags, `-i` is assumed. Examples:

```sh
./sample_account -ilfm 10        # id, lastname, firstname, mail × 10 rows
./sample_account --id --age 5    # id, age × 5 rows
```

Note: the long-option table has typos preserved from upstream (`telehpne` for `-t`, and `-o`/`-y`/`-r` lack long names). Don't "fix" these without confirming — external scripts may rely on the short flags.

## Architecture

Three data-loader classes, each reading one CSV at construction and exposing accessor methods that map an arbitrary `int` (typically `rand()`) onto a record via modulo. `main()` constructs one of each, then loops `max` times printing the requested columns.

- **`Account`** (`include/accounts.h`, `source/accounts.cpp`) — loads `data/sample_account.csv` (kanji/kana/romaji last+first, gender, blood). `LastName`/`FirstName` return *two* CSV fields (kanji,kana) joined by a comma — so `-l` actually emits two columns. `mailAddress` builds `<first_romaji>_<last_romaji>@example.com` (`DOMAIN` macro).
- **`Prefecture`** (`include/addresses.h`, `source/addresses.cpp`) — loads `prefectures.csv` (47 fixed entries) and `address.csv` (~190K Japan-Post-derived rows). `setPrefNumber(rand())` picks a prefecture *weighted by population* (so output distribution matches real Japan demographics); the same index then feeds `getPrefecture/getWard/getCity` so the three columns stay consistent for a row. The 2-arg overloads use `getZip(pref, num)` to constrain ward/city lookups to the chosen prefecture's slice of `address.csv`.
- **`Random`** (`include/random.h`, `source/random.cpp`) — loads `data/ages.csv` (age-group populations). `getAge` picks an age weighted by population; `getReward` derives an income-like figure from age group; `getRandomYear/Month/Day` decompose `rand_time` (set per-row via `setTime()`) into a valid date.

`source/sample_account.cpp` is the only place that ties options to methods. Per-row randomness comes from `rand()` seeded once with `time(NULL)` plus a `hash_seed` for the (currently disabled) hash key path.

## Data Files

CSVs are committed; do not regenerate casually.

- `data/sample_account.csv` — `last_kanji,last_kana,last_romaji,first_kanji,first_kana,first_romaji,gender,blood`
- `data/prefectures.csv` — `code,name,population` (47 rows; `zips` count is computed at load time from `address.csv`)
- `data/address.csv` — `pref_code,prefecture,ward,city` (sorted by `pref_code`; loader assumes contiguous grouping when computing `zips`)
- `data/ages.csv` — `generation,population` (the 4-column appearance in `head` is misleading — only the first two fields are parsed; `start` is computed as a running sum)
- `tool/convert-address.sh` regenerates `address.csv` from raw Japan Post CSV (requires `nkf`).

## Notable Constraints / Gotchas

- `hash.h`/`GenerateHash` is declared but the implementation file is missing; `-Iinclude` and the `// #include "hash.h"` line are dormant — re-enabling SHA features needs a new `source/hash.cpp` and OpenSSL linking (`-lcrypto`) added to `LDFLAGS`.
- `old/sample_account.c` is a historical C version, kept for reference; `.gitignore` already excludes `/old` from broader patterns. Don't modify it.
- The version macro lives at `include/sample_account.h:4` (`#define VERSION 0.4.7`) — bump it when releasing.
- Recent commits follow short imperative style (`add date`, `fix illegal date`, `update version`); keep the same tone for new commits.
