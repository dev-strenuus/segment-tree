# C++ implementation of Segment Tree

This repo provides a C++ implementation of a segment tree to answer interval queries on
an array of fixed size whose values can be updated by the user.

A query `Q(l, r, op)` asks to aggregate all the values inside the interval `[l,r]` with
the aggregator `op`. While `U(p, v)` updates the array with value `v` in position `p`.

For instance, given the array `[0,1,2,3,4]`
- `Q(0, 2, max)` -> 2
- `U(0, 7)`
- `Q(0, 2, max)` -> 7

A segment tree allows to perform both queries and updates in `O(logN)`.

This repo implements both a slow and fast `QueryUpdateSolver`. You can see an example of usage in `main.cpp` where the fast solver that uses segment tree is tested against the slow solver.

Currently only the `min` and `max` aggregators are implemented.