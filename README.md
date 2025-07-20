# Competitive Programming Templates

A comprehensive collection of templates and utilities for competitive programming contests like ICPC, ACM, and other international competitions.

## File Structure

```
CP-Templates/
├── README.md                           # Main documentation
├── CONTEST_GUIDE.md                    # Contest strategies and tips
├── build.bat                           # Windows build script
│
├── templates/                          # Basic templates for quick setup
│   ├── main.cpp                       # Main template with utilities
│   └── contest_setup.cpp               # Quick contest problem setup
│
├── data_structures/                    # Essential data structures
│   ├── dsu.cpp                        # Disjoint Set Union
│   ├── segment_tree.cpp               # Segment Tree with lazy propagation
│   ├── fenwick_tree.cpp               # Binary Indexed Tree (1D & 2D)
│   ├── heavy_light_decomposition.cpp  # Tree path queries
│   ├── centroid_decomposition.cpp     # Tree divide-and-conquer
│   ├── link_cut_tree.cpp              # Dynamic tree connectivity
│   └── persistent_segment_tree.cpp    # Historical queries
│
├── graph/                             # Graph algorithms
│   ├── dfs_bfs.cpp                    # DFS, BFS traversal algorithms
│   ├── shortest_paths.cpp             # Dijkstra, Bellman-Ford, Floyd-Warshall
│   ├── topological_sort.cpp           # Topological sorting algorithms
│   ├── strongly_connected_components.cpp # Tarjan's and Kosaraju's algorithms
│   ├── minimum_spanning_tree.cpp      # Kruskal's and Prim's MST algorithms
│   ├── max_flow.cpp                   # Maximum flow algorithms
│   ├── min_cost_flow.cpp              # Minimum cost maximum flow
│   ├── bipartite_matching.cpp         # Bipartite matching algorithms
│   ├── network_flows.cpp              # General network flow utilities
│   └── cycle_detection.cpp            # Cycle detection in graphs
│
├── math/                              # Mathematical utilities
│   ├── basic_arithmetic.cpp           # Basic arithmetic operations and utilities
│   ├── number_theory_functions.cpp    # GCD, LCM, primes, modular arithmetic
│   ├── prime_algorithms.cpp           # Prime generation and testing algorithms
│   ├── combinatorics.cpp              # Factorials, combinations, Catalan numbers
│   ├── matrix_operations.cpp          # Matrix operations and linear algebra
│   ├── fft_ntt.cpp                    # FFT, NTT for polynomial operations
│   ├── discrete_algorithms.cpp        # Discrete mathematics algorithms
│   └── advanced_math.cpp              # Advanced mathematical functions
│
├── strings/                           # String processing
│   ├── string_utilities.cpp           # Basic string manipulation utilities
│   ├── string_hashing.cpp             # Rolling hash and string hashing
│   ├── pattern_matching.cpp           # KMP, Z-algorithm, string matching
│   ├── suffix_structures.cpp          # Suffix arrays, suffix trees
│   ├── palindrome_algorithms.cpp      # Palindrome detection and algorithms
│   └── trie_structures.cpp            # Trie and related data structures
│
├── geometry/                          # Computational geometry
│   ├── basic_structures.cpp           # Points, vectors, basic geometric structures
│   ├── line_operations.cpp            # Line operations and intersections
│   ├── circle_operations.cpp          # Circle operations and algorithms
│   ├── polygon_operations.cpp         # Polygon operations and algorithms
│   └── advanced_algorithms.cpp        # Convex hull, line sweep, advanced geometry
│
├── algorithms/                        # Common and advanced algorithms
│   ├── common_algorithms.cpp          # Binary search, sorting, LIS, LCS
│   ├── sorting_algorithms.cpp         # Various sorting algorithm implementations
│   ├── searching_algorithms.cpp       # Binary search and searching variants
│   ├── sequence_algorithms.cpp        # Sequence processing algorithms
│   ├── dynamic_programming.cpp        # Basic DP patterns and algorithms
│   ├── digit_dp.cpp                   # Digit DP for number constraints
│   ├── sos_dp.cpp                     # Sum Over Subsets DP
│   ├── convex_hull_trick.cpp          # CHT and Li Chao Tree
│   ├── bitmask_dp.cpp                 # Bitmask DP utilities and TSP
│   ├── dp_optimizations.cpp           # Divide & Conquer, Knuth-Yao DP
│   ├── profile_dp.cpp                 # Profile DP for grid problems
│   ├── sliding_window.cpp             # Sliding window algorithms
│   └── string_algorithms.cpp          # String processing algorithms
│
└── utils/                             # Utility functions and macros
    ├── utilities.cpp                  # Main utilities collection
    ├── io_utilities.cpp               # I/O helpers and formatting
    ├── math_utilities.cpp             # Mathematical utility functions
    └── general_utilities.cpp          # General purpose utility functions
```

## Recent Updates

### Code Quality Improvements (Latest)
- **Compiler Warning Fixes**: Resolved all signed/unsigned comparison warnings across the entire codebase
- **Type Safety**: Consistently cast `.size()` and `.length()` calls to `(int)` in loop conditions
- **Compilation Fixes**: Fixed namespace issues, private/public keyword misuse, and const-correctness
- **Cross-platform Compatibility**: Ensured all code compiles cleanly with modern C++ compilers

## Key Features

### Core Templates
- **main.cpp**: Complete template with fast I/O, common utilities, and debugging macros
- **contest_setup.cpp**: Quick setup for contest problems with minimal boilerplate

### Advanced Data Structures
- **Heavy-Light Decomposition**: Efficient tree path queries and updates
- **Centroid Decomposition**: Tree divide-and-conquer for distance queries
- **Link-Cut Tree**: Dynamic tree connectivity and path operations
- **Persistent Segment Tree**: Historical queries and functional data structures
- **Advanced Segment Trees**: Lazy propagation, range updates, and specialized variants
- **Balanced Trees**: Self-balancing tree implementations
- **Merge Sort Tree**: Range query data structure
- **Specialized Structures**: Problem-specific data structure implementations
- Segment Trees with lazy propagation and range updates
- Fenwick Trees (Binary Indexed Trees) for prefix sum queries
- Disjoint Set Union with path compression and union by rank

### Network Flows
- **Dinic's Algorithm**: Maximum flow with O(V²E) complexity
- **Min-Cost Max-Flow**: Cost-optimal flow solutions
- **Hungarian Algorithm**: Assignment problem solver
- **Bipartite Matching**: Kuhn's and Hopcroft-Karp algorithms
- **General Matching**: Blossom algorithm for maximum matching

### Advanced Mathematics
- **FFT/NTT**: Fast polynomial multiplication and convolution
- **Matrix Operations**: Multiplication, determinant, inverse, linear algebra
- **Number Theory**: Discrete logarithm, integer factorization, primality testing
- **Multiplicative Functions**: Euler's totient, Möbius function, divisor functions

### Advanced Dynamic Programming
- **Digit DP**: Counting numbers with specific digit constraints
- **SOS DP**: Sum Over Subsets for efficient subset enumeration
- **Convex Hull Trick**: DP optimization for convex cost functions
- **Divide and Conquer DP**: Optimization for specific recurrence patterns
- **Knuth-Yao Optimization**: Matrix chain-like DP optimization
- **Bitmask DP**: TSP, assignment problems, and subset enumeration
- **Profile DP**: Grid tiling and complex state space problems

### String Algorithms
- **Suffix Arrays**: O(n log n) construction with LCP arrays
- **Aho-Corasick**: Multiple pattern matching automaton
- **Suffix Trees**: Ukkonen's linear-time construction
- **Palindromic Trees**: Eertree for palindrome queries
- **Lyndon Words**: Duval's algorithm for string factorization
- **Multiple Hashing**: Robust string matching and comparison
- KMP, Z-algorithm, and rolling hash
- Trie data structure for prefix operations

### Computational Geometry
- **Convex Hull**: Graham scan, Andrew's monotone chain
- **Line Sweep**: Rectangle union, intersection algorithms
- **Rotating Calipers**: Diameter, width, closest/farthest pair
- **Polygon Operations**: Area, containment, intersection, union
- **Delaunay Triangulation**: Optimal triangulation of point sets
- **Voronoi Diagrams**: Nearest neighbor partitioning
- Basic geometric primitives and robust intersection algorithms

## Usage

### Quick Start
1. **Copy Template**: Start with `templates/main.cpp` for a new problem
2. **Include Algorithms**: Add specific algorithm files as needed
3. **Compile**: Use `build.bat` on Windows or compile manually with optimizations
4. **Debug**: Utilize built-in debugging macros and utilities

### Example Usage
```cpp
#include "templates/main.cpp"
#include "data_structures/segment_tree.cpp"
#include "algorithms/common_algorithms.cpp"

int main() {
    fast_io();
    
    int n;
    cin >> n;
    vector<int> a(n);
    for(int i = 0; i < n; i++) cin >> a[i];
    
    // Use segment tree for range queries
    SegmentTree st(a);
    
    return 0;
}
```

### Compilation
```bash
# Windows (using build.bat)
build.bat solution.cpp

# Manual compilation with optimizations (recommended flags)
g++ -std=c++17 -O2 -Wall -Wextra -Wsign-compare -o solution solution.cpp

# Debug compilation with all warnings
g++ -std=c++17 -g -DDEBUG -Wall -Wextra -Wsign-compare -o solution_debug solution.cpp

# Contest-safe compilation (no warnings)
g++ -std=c++17 -O2 -w -o solution solution.cpp
```

### Customization Guidelines

#### Template Modification
- Adjust `MOD` constant for problem-specific modular arithmetic
- Modify I/O functions for different input/output formats
- Add problem-specific utility functions to the template

#### Algorithm Selection
- Choose appropriate data structures based on constraints
- Consider time/space complexity trade-offs
- Use advanced algorithms only when necessary

#### Debugging
- Use `debug()` macro for variable inspection
- Enable `TRACE` for algorithm step-by-step execution
- Utilize `Timer` class for performance profiling

### Testing Setup

#### Local Testing
```bash
# Create test cases
echo "5\n1 2 3 4 5" > input.txt

# Run solution
./solution < input.txt > output.txt

# Compare with expected output
diff output.txt expected.txt
```

#### Stress Testing
```cpp
// Include random generator from utilities
#include "utils/utilities.cpp"

int main() {
    for(int test = 0; test < 1000; test++) {
        // Generate random test case
        int n = rnd(1, 100);
        vector<int> a = generate_random_array(n, 1, 1000);
        
        // Compare brute force vs optimized solution
        auto result1 = brute_force(a);
        auto result2 = optimized_solution(a);
        
        assert(result1 == result2);
    }
    return 0;
}
```

### Contest Compatibility

#### Platform Support
- **Codeforces**: Full compatibility with C++17/C++20
- **AtCoder**: Optimized for AtCoder's judge system
- **ICPC**: Follows ICPC programming standards
- **Google Code Jam**: Supports multiple test cases format

#### Performance Considerations
- All algorithms tested with maximum constraints
- Memory usage optimized for typical contest limits (256MB)
- Time complexity verified for 1-2 second time limits
- Robust against edge cases and overflow

#### Contest Strategy
- Use `contest_setup.cpp` for rapid problem setup
- Keep frequently used algorithms in separate files
- Maintain a personal subset of most-used templates
- Practice with the exact template structure used in contests

## File Descriptions

### Core Templates
- **main.cpp**: Complete competitive programming template with fast I/O, debugging macros, common utilities, and mathematical functions
- **contest_setup.cpp**: Minimal template for quick problem setup during contests

### Data Structures

#### Basic Data Structures
- **dsu.cpp**: Disjoint Set Union with path compression and union by rank
- **segment_tree.cpp**: Segment tree with lazy propagation, range updates, and customizable operations
- **fenwick_tree.cpp**: Binary Indexed Tree for prefix sums, 1D and 2D variants

#### Advanced Tree Structures
- **heavy_light_decomposition.cpp**: Decomposes trees for efficient path queries and updates
- **centroid_decomposition.cpp**: Tree divide-and-conquer for distance-based queries
- **link_cut_tree.cpp**: Dynamic tree connectivity with link, cut, and path operations
- **persistent_segment_tree.cpp**: Functional segment tree supporting historical queries

### Graph Algorithms

#### Basic Graph Algorithms
- **basic_algorithms.cpp**: DFS, BFS, shortest paths (Dijkstra, Bellman-Ford, Floyd-Warshall)

#### Advanced Graph Algorithms
- **advanced_algorithms.cpp**: Topological sorting, strongly connected components (Tarjan's, Kosaraju's), minimum spanning tree (Kruskal's, Prim's)

#### Network Flows
- **network_flows.cpp**: 
  - Dinic's algorithm for maximum flow
  - Min-cost max-flow with SPFA
  - Hungarian algorithm for assignment problems
  - Bipartite matching (Kuhn's algorithm, Hopcroft-Karp)
  - General matching (Blossom algorithm)

### Mathematics

#### Number Theory
- **number_theory.cpp**: GCD, LCM, extended Euclidean algorithm, modular arithmetic, prime generation (Sieve of Eratosthenes), prime factorization

#### Combinatorics
- **combinatorics.cpp**: Factorials, combinations, permutations, Catalan numbers, Stirling numbers, inclusion-exclusion principle

#### Advanced Mathematics
- **advanced_math.cpp**:
  - FFT/NTT for polynomial multiplication
  - Matrix operations (multiplication, determinant, inverse, exponentiation)
  - Linear algebra (Gaussian elimination, linear system solving)
  - Discrete logarithm (Baby-step Giant-step)
  - Integer factorization (Pollard's rho, trial division)
  - Primality testing (Miller-Rabin)
  - Multiplicative functions (Euler's totient, Möbius function)

### String Processing

#### Basic String Algorithms
- **string_algorithms.cpp**: KMP pattern matching, Z-algorithm, polynomial rolling hash, Trie data structure, basic suffix array

#### Advanced String Algorithms
- **advanced_strings.cpp**:
  - Suffix Array with O(n log n) construction and LCP array
  - Aho-Corasick algorithm for multiple pattern matching
  - Suffix Tree (Ukkonen's algorithm)
  - Palindromic Tree (Eertree)
  - Lyndon words and Duval's algorithm
  - Multiple hash string matching

### Computational Geometry

#### Basic Geometry
- **computational_geometry.cpp**: Point and vector operations, line intersections, polygon area, point-in-polygon tests, convex hull (Graham scan)

#### Advanced Geometry
- **advanced_geometry.cpp**:
  - Multiple convex hull algorithms (Graham scan, Andrew's algorithm)
  - Line sweep for rectangle union
  - Rotating calipers for diameter and closest pair
  - Polygon operations (area, containment, intersection)
  - Delaunay triangulation and Voronoi diagrams
  - Advanced closest pair algorithms

### Common Algorithms

#### Basic Algorithms
- **common_algorithms.cpp**: Binary search variants, sorting algorithms, Longest Increasing Subsequence (LIS), Longest Common Subsequence (LCS), basic dynamic programming patterns

#### Advanced Dynamic Programming
- **digit_dp.cpp**: Digit DP for counting numbers with digit constraints
- **sos_dp.cpp**: Sum Over Subsets DP for efficient subset enumeration
- **convex_hull_trick.cpp**: CHT and Li Chao Tree for DP optimization
- **bitmask_dp.cpp**: Bitmask DP utilities, TSP, assignment problems
- **dp_optimizations.cpp**: Divide and Conquer DP, Knuth-Yao optimization
- **profile_dp.cpp**: Profile DP for grid tiling and complex state problems

### Utilities
- **utilities.cpp**: I/O helpers, debugging macros, random number generation, timer utilities, mathematical helper functions

## Contributing

Contributions are welcome! Please ensure:
- Code follows existing style and conventions
- All algorithms are well-tested and optimized for competitive programming
- Documentation includes complexity analysis and usage examples
- New algorithms include comprehensive test cases
- Performance is verified against maximum constraints

## License

This project is open source and available under the MIT License.