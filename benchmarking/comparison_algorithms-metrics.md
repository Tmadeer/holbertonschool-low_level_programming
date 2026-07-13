# Algorithmic Efficiency Comparison

## Raw Results

### Run 1
Naive algorithm result: 625025000
Naive algorithm time: 2.645725 seconds
Single-pass algorithm result: 25000
Single-pass algorithm time: 0.000080 seconds

### Run 2
Naive algorithm result: 625025000
Naive algorithm time: 2.011052 seconds
Single-pass algorithm result: 25000
Single-pass algorithm time: 0.000080 seconds

### Run 3
Naive algorithm result: 625025000
Naive algorithm time: 2.039459 seconds
Single-pass algorithm result: 25000
Single-pass algorithm time: 0.000087 seconds

## Averages

- Naive algorithm average: 2.232079 seconds
- Single-pass algorithm average: 0.0000823 seconds

## Analysis

The naive algorithm is approximately **27,110 times slower** than the
single-pass algorithm.

This difference is explained by algorithmic complexity: the naive
implementation uses a nested loop, re-checking previously counted elements
on every outer iteration (O(n²) behavior), resulting in roughly
ARRAY_SIZE² = 2,500,000,000 total comparisons for ARRAY_SIZE = 50000. The
single-pass implementation checks each element exactly once (O(n)
behavior), resulting in only 50,000 comparisons.

As the array size grows, this gap widens dramatically, since quadratic
growth vastly outpaces linear growth. This demonstrates why algorithmic
complexity matters far more than raw CPU speed or micro-optimizations —
choosing the right approach (O(n) vs O(n²)) had a far greater impact on
performance than anything else could have.
