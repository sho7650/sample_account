#!/bin/sh
# Snapshot tests for sample_account.
# Each scenario pins SAMPLE_ACCOUNT_SEED for reproducibility, runs the binary,
# and diffs stdout against a checked-in expected file. Any unintended output
# drift fails the test.

set -e

cd "$(dirname "$0")/.."

if [ ! -x ./sample_account ]; then
  echo "FAIL: ./sample_account not built. Run 'make' first." >&2
  exit 1
fi

tmp=$(mktemp)
trap 'rm -f "$tmp"' EXIT

run_case() {
  scenario=$1
  expected=$2
  shift 2
  SAMPLE_ACCOUNT_SEED=42 SAMPLE_ACCOUNT_NOW=1700000000 ./sample_account "$@" > "$tmp"
  if diff -u "$expected" "$tmp"; then
    printf 'PASS: %s\n' "$scenario"
  else
    printf 'FAIL: %s\n' "$scenario" >&2
    exit 1
  fi
}

run_case "all flags (long output)"    tests/expected/all-flags-seed-42.csv     -ilfmatpwcgbdorynq 5
run_case "id+name+mail (-ilfm)"       tests/expected/ilfm-seed-42.csv          -ilfm 5
run_case "default (no flags)"         tests/expected/default-seed-42.csv       3
run_case "long aliases"               tests/expected/long-aliases-seed-42.csv  --telephone --agegroup --birthyear 4

echo "All snapshot tests passed."
