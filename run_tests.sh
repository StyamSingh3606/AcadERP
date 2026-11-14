#!/bin/bash

set -u

make || exit 1

passed=0
failed=0

for input in tests/T*.in; do
    test_name=$(basename "$input" .in)
    expected="tests/${test_name}.out"
    actual="/tmp/${test_name}.actual"

    if [[ ! -f "$expected" ]]; then
        echo "FAIL: $test_name (missing expected output)"
        ((failed++))
        continue
    fi

    ./acaderp < "$input" > "$actual" 2>/dev/null

    if diff -u "$expected" "$actual" > /dev/null; then
        echo "PASS: $test_name"
        ((passed++))
    else
        echo "FAIL: $test_name"
        diff -u "$expected" "$actual"
        ((failed++))
    fi
done

echo "--------------------"
echo "Passed: $passed"
echo "Failed: $failed"

if [[ "$failed" -ne 0 ]]; then
    exit 1
fi