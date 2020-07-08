#include <iostream>
#include <vector>
#include <climits>
#include <cstdlib>

using namespace std;

struct Pair {
        float i;
        float j;
        int index;
};

struct Matching {
        vector<Pair> matching;
        float weight;
};

/* Prototipos */
vector<Pair> get_blocks (vector<int>);
Matching greedy_matching (vector<int>, vector<int>);
Matching min_matching_recursive (vector<int>, vector<int>);
Matching min_matching_memoized (vector<int>, vector<int>);
Matching opt_solution (vector<Pair>, vector<Pair>);
Matching opt_solution_mem (vector<Pair>, vector<Pair>, bool left);
vector<Pair> merge_matchings (vector<Pair>, vector<Pair>);
vector<int> string_to_vector (string);
void print_matching (Matching);
