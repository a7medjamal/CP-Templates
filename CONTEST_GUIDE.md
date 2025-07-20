# Competitive Programming Contest Guide

## Pre-Contest Preparation

### Environment Setup
1. **IDE/Editor**: Use a familiar editor (VS Code, CLion, Sublime Text)
2. **Compiler**: Ensure g++ is installed with C++17 support
3. **Templates**: Have your main template ready and tested
4. **Snippets**: Prepare code snippets for common algorithms
5. **Reference**: Keep algorithm complexity cheat sheet handy

### Essential Templates to Review
- Main template with fast I/O
- Graph algorithms (DFS, BFS, Dijkstra)
- Data structures (Segment Tree, DSU, Fenwick Tree)
- String algorithms (KMP, Z-algorithm)
- Mathematical functions (GCD, LCM, Modular arithmetic)
- Geometry basics (if contest includes geometry)

## During Contest Strategy

### Reading Problems
1. **Read all problems first** (spend 10-15 minutes)
2. **Identify easy problems** and solve them first
3. **Note constraints** - they hint at the expected solution complexity
4. **Look for patterns** - similar problems you've solved before

### Problem Solving Order
1. **Implementation problems** (usually A, B)
2. **Standard algorithm problems** (C, D)
3. **Advanced/novel problems** (E, F, G+)

### Time Management
- **First 30 minutes**: Solve 2-3 easy problems
- **Next 60 minutes**: Focus on medium difficulty problems
- **Last 90 minutes**: Attempt harder problems or debug
- **Last 30 minutes**: Double-check solutions, fix edge cases

### Coding Best Practices

#### Template Usage
```cpp
#include <bits/stdc++.h>
using namespace std;

// Your standard template here
typedef long long ll;
#define fast_io ios_base::sync_with_stdio(false); cin.tie(NULL);

void solve() {
    // Solution code
}

int main() {
    fast_io;
    int t = 1;
    // cin >> t;  // Uncomment for multiple test cases
    while(t--) solve();
    return 0;
}
```

#### Common Mistakes to Avoid
1. **Integer overflow** - use `long long` when needed
2. **Array bounds** - always check array access
3. **Uninitialized variables** - initialize all variables
4. **Wrong data types** - match problem constraints
5. **Edge cases** - test with minimum/maximum inputs
6. **Multiple test cases** - reset variables between test cases

### Debugging Techniques

#### Local Testing
```cpp
#ifdef LOCAL
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
```

#### Debug Macros
```cpp
#ifdef LOCAL
    #define dbg(x) cerr << #x << " = " << x << endl
#else
    #define dbg(x)
#endif
```

#### Test Cases
- **Sample cases**: Always test provided examples
- **Edge cases**: Empty input, single element, maximum constraints
- **Corner cases**: All same elements, sorted/reverse sorted arrays

## Problem Categories & Approaches

### Implementation (A, B problems)
- **Simulation**: Follow problem statement exactly
- **String manipulation**: Use built-in string functions
- **Basic math**: Arithmetic operations, simple formulas
- **Array operations**: Sorting, searching, basic transformations

### Algorithms (C, D problems)
- **Greedy**: Local optimal choices lead to global optimum
- **Dynamic Programming**: Optimal substructure and overlapping subproblems
- **Graph algorithms**: BFS/DFS for connectivity, shortest paths
- **Binary search**: On answer or in sorted arrays
- **Two pointers**: For array problems with specific conditions

### Data Structures (D, E problems)
- **Segment trees**: Range queries and updates
- **Fenwick trees**: Prefix sum queries
- **DSU**: Connected components, cycle detection
- **Priority queues**: Dijkstra, event processing
- **Hash maps**: Frequency counting, fast lookups

### Advanced Topics (E+ problems)
- **Number theory**: Modular arithmetic, prime factorization
- **Combinatorics**: Permutations, combinations, inclusion-exclusion
- **String algorithms**: KMP, Z-algorithm, suffix arrays
- **Geometry**: Convex hull, line intersections
- **Game theory**: Nim, minimax

## Complexity Analysis

### Time Complexity Guidelines
- **n ≤ 10**: O(n!), O(n^6)
- **n ≤ 20**: O(2^n), O(n^5)
- **n ≤ 100**: O(n^4)
- **n ≤ 1,000**: O(n^3)
- **n ≤ 10,000**: O(n^2)
- **n ≤ 100,000**: O(n log n)
- **n ≤ 1,000,000**: O(n)
- **n > 1,000,000**: O(log n) or O(1)

### Space Complexity
- Usually less critical than time
- Watch out for 2D arrays with large dimensions
- Use `vector` instead of fixed arrays when possible

## Contest-Specific Tips

### ICPC Style
- **Team coordination**: Divide problems based on strengths
- **One computer**: Plan who codes what and when
- **Paper work**: Use paper for complex algorithms
- **Balloon strategy**: Solve easy problems first for early balloons

### Online Contests (Codeforces, AtCoder)
- **Penalty system**: Avoid wrong submissions
- **Hacking**: In Codeforces, prepare for hacking phase
- **Rating**: Focus on solving problems correctly rather than speed

### IOI Style
- **Partial scoring**: Optimize for maximum points
- **Subtasks**: Solve easier subtasks first
- **Time limit**: Usually more generous, focus on correctness

## Emergency Procedures

### When Stuck
1. **Re-read problem**: Make sure you understand correctly
2. **Check constraints**: Might hint at different approach
3. **Try examples manually**: Trace through your algorithm
4. **Simplify**: Solve easier version first
5. **Move on**: Don't spend too much time on one problem

### Last 30 Minutes
1. **Review all solutions**: Check for obvious bugs
2. **Test edge cases**: Minimum/maximum inputs
3. **Check data types**: Ensure no overflow
4. **Verify I/O format**: Match exactly with problem statement

### Common Last-Minute Fixes
- Add `long long` for large numbers
- Initialize arrays/variables
- Handle empty input cases
- Check array bounds
- Verify loop conditions

## Post-Contest

### Learning
1. **Read editorials**: Understand optimal solutions
2. **Implement missing solutions**: Practice new techniques
3. **Analyze mistakes**: Learn from wrong submissions
4. **Update templates**: Add new useful code snippets

### Practice
- **Virtual contests**: Simulate contest environment
- **Problem sets**: Focus on weak areas
- **Upsolving**: Solve problems you couldn't during contest

Remember: **Practice makes perfect!** The more contests you participate in, the better you'll become at managing time, recognizing patterns, and implementing solutions quickly and correctly.