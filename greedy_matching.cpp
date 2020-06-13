#include <iostream>
#include <vector>
#include <climits>
#include <cmath>
using namespace std;

struct Pair {
	int i;
	int j;
};

struct Matching {
	vector<Pair> matching;
	float weight;
};

/* Prototipos */
vector<Pair> get_blocks (vector<int>);
vector<Pair> greedy_matching (vector<int>, vector<int>);
Matching min_matching_recursive (vector<int>, vector<int>);
Matching opt_solution (vector<Pair>, vector<Pair>);
Matching opt_solution_mem (vector<Pair>, vector<Pair>);
vector<Pair> merge_matchings (vector<Pair>, vector<Pair>);

vector<vector<Matching>> mem;

vector<Pair> get_blocks (vector<int> A) {
	vector<Pair> blocks;
	bool flag = false;
	Pair block;
	for (int i = 0; i < A.size (); i++) {
		if (A[i] == 1) {
			if (!flag) {
				flag = true;
				block.i = i;
				if (i == A.size () - 1)	{
					block.j = i;
					blocks.push_back (block);
				}
			}
		}
		else if (A[i] == 0) {
			if (flag) {
				flag = false;
				block.j = i - 1;
				blocks.push_back (block);
			}
		}
	}
	return blocks;
}

vector<Pair> greedy_matching (vector<int> A, vector<int> B) {
	vector<Pair> result;

	vector<Pair> blocks_A = get_blocks (A);
	vector<Pair> blocks_B = get_blocks (B);

	int i = 0, j = 0, k = 0;
	int n = blocks_A.size ();
	int m = blocks_B.size ();

	int max;
	if (n > m) {
		max = n;
		m -=1;
	}
	else {
		max = m;
		n -=1;
	}
	Pair match;
	while (i < max && j < max) {
		match.i = blocks_A[i].i;
		match.j = blocks_B[j].j;
		result.push_back (match);

		if (i < n)
			i++;
		if (j < m)
			j++;
	}
	return result;
}

Matching min_matching_recursive (vector<int> A, vector<int> B) {
    vector<Pair> result;

    vector<Pair> blocks_A = get_blocks (A);
    vector<Pair> blocks_B = get_blocks (B);

    Matching min_matching = opt_solution (blocks_A, blocks_B);

    return min_matching;
}

Matching opt_solution (vector<Pair> A, vector<Pair> B) {
	Matching min_match;
	int i = A.size () - 1;
	int j = B.size () - 1;
	Matching min_agrupacion;
	Matching min_division;

	min_agrupacion.weight = INFINITY;
	min_division.weight = INFINITY;

	if (A.size () == 1 and B.size () == 1) {
		Pair match;
		match.i = A[0].i;
		match.j = B[0].j;
		min_match.matching.push_back (match);
		min_match.weight = A[0].i / B[0].j;
		return min_match;
	}
		
	if (A.size () == 1 and B.size () > 1) {
		Pair match;
		int sum = 0;
		match.i = A[0].i;
		for (auto it = 0; it < B.size (); it++) {
			match.j = B[it].j;
			min_match.matching.push_back (match);
			sum += B[it].j;
		}
		min_match.weight = A[0].i / sum;
		return min_match;
	}
	if (B.size () == 1 and A.size () > 1){
		Pair match;
		int sum = 0;
		match.j = B[0].j;
		for (auto it = 0; it < A.size (); it++){
			match.i = A[it].i;
			min_match.matching.push_back (match);
			sum += A[it].i;
		}
		min_match.weight = sum / B[0].j;
		return min_match;
	}
	
	for (int k = i - 1; k >= 0; k--) {
		vector<Pair> left_A (A.begin (), A.begin () + k + 1);
		vector<Pair> right_A (A.begin () + k + 1, A.end ());
		
		vector<Pair> left_B (B.begin (), B.end () - 1);
		vector<Pair> right_B; right_B.push_back (B[B.size () - 1]);
		
		Matching min_left = opt_solution (left_A, left_B);
		Matching min_right = opt_solution (right_A, right_B);
		
		Matching merge;
		merge.matching = merge_matchings (min_left.matching, min_right.matching);
		merge.weight = min_left.weight + min_right.weight;
		if (merge.weight < min_agrupacion.weight)
			min_agrupacion = merge;
		for (auto h = 0; h < merge.matching.size (); h++)
        	{
                	cout << "(" << merge.matching[h].i << "," << merge.matching[h].j << ")" << " ";
        	}
		cout << merge.weight << endl;
	}
	for (int k = j - 1; k >= 0; k--) {
		vector<Pair> left_A (A.begin (), A.end () - 1);
		vector<Pair> right_A; right_A.push_back (A[A.size () - 1]);

		vector<Pair> left_B (B.begin (), B.begin () + k + 1);
		vector<Pair> right_B (B.begin () + k + 1, B.end ());
		
		Matching min_left = opt_solution (left_A, left_B);
		Matching min_right = opt_solution (right_A, right_B);
		
		Matching merge;
		merge.matching = merge_matchings (min_left.matching, min_right.matching);
		merge.weight = min_left.weight + min_right.weight;
		if (merge.weight < min_division.weight)
			min_division = merge;
		for (auto h = 0; h < merge.matching.size (); h++)
        	{
                	cout << "(" << merge.matching[h].i << "," << merge.matching[h].j << ")" << " ";
        	}
		cout << merge.weight << endl;
	}

	return min_agrupacion.weight < min_division.weight ? min_agrupacion : min_division;
}

vector<Pair> merge_matchings (vector<Pair> left, vector<Pair> right) {
	for (int i = 0; i < right.size (); i++)
		left.push_back (right[i]);
	return left;
}

Matching min_matching_memoized (vector<int> A, vector<int> B) {
    vector<Pair> result;

    vector<Pair> blocks_A = get_blocks (A);
    vector<Pair> blocks_B = get_blocks (B);

    mem.resize (blocks_A.size ());
    for (int i = 0; i < blocks_A.size (); i++)
                mem[i].resize (blocks_B.size ());

    Matching min_matching = opt_solution_mem (blocks_A, blocks_B);

    return min_matching;
}

Matching opt_solution_mem (vector<Pair> A, vector<Pair> B) {
	Matching min_match;
	int i = A.size () - 1;
	int j = B.size () - 1;
	Matching min_agrupacion;
	Matching min_division;

	min_agrupacion.weight = INFINITY;
	min_division.weight = INFINITY;
	
	if (mem[i][j].weight != 0) {
		return mem[i][j];
	}
	if (A.size () == 1 and B.size () == 1) {
		Pair match;
		match.i = A[0].i;
		match.j = B[0].j;
		min_match.matching.push_back (match);
		min_match.weight = A[0].i / B[0].j;
		return min_match;
	}
		
	if (A.size () == 1 and B.size () > 1) {
		Pair match;
		int sum = 0;
		match.i = A[0].i;
		for (auto it = 0; it < B.size (); it++) {
			match.j = B[it].j;
			min_match.matching.push_back (match);
			sum += B[it].j;
		}
		min_match.weight = A[0].i / sum;
		return min_match;
	}

	if (B.size () == 1 and A.size () > 1){
		Pair match;
		int sum = 0;
		match.j = B[0].j;
		for (auto it = 0; it < A.size (); it++){
			match.i = A[it].i;
			min_match.matching.push_back (match);
			sum += A[it].i;
		}
		min_match.weight = sum / B[0].j;
		return min_match;
	}
	
	for (int k = i - 1; k >= 0; k--) {
		vector<Pair> left_A (A.begin (), A.begin () + k + 1);
		vector<Pair> right_A (A.begin () + k + 1, A.end ());
		
		vector<Pair> left_B (B.begin (), B.end () - 1);
		vector<Pair> right_B; right_B.push_back (B[B.size () - 1]);
		
		Matching min_left = opt_solution_mem (left_A, left_B);
		Matching min_right = opt_solution_mem (right_A, right_B);
		
		Matching merge;
		merge.matching = merge_matchings (min_left.matching, min_right.matching);
		merge.weight = min_left.weight + min_right.weight;
		if (merge.weight < min_agrupacion.weight)
			min_agrupacion = merge;
		for (int h = 0; h < merge.matching.size (); h++)
        	{
                	cout << "(" << merge.matching[h].i << "," << merge.matching[h].j << ")" << " ";
        	}
		cout << merge.weight << endl;
		mem[k][j - 1] = merge;
	}
	for (int k = j - 1; k >= 0; k--) {
		vector<Pair> left_A (A.begin (), A.end () - 1);
		vector<Pair> right_A; right_A.push_back (A[A.size () - 1]);

		vector<Pair> left_B (B.begin (), B.begin () + k + 1);
		vector<Pair> right_B (B.begin () + k + 1, B.end ());
		
		Matching min_left = opt_solution (left_A, left_B);
		Matching min_right = opt_solution (right_A, right_B);
		
		Matching merge;
		merge.matching = merge_matchings (min_left.matching, min_right.matching);
		merge.weight = min_left.weight + min_right.weight;
		if (merge.weight < min_division.weight)
			min_division = merge;
		for (int h = 0; h < merge.matching.size (); h++)
        	{
                	cout << "(" << merge.matching[h].i << "," << merge.matching[h].j << ")" << " ";
        	}
		cout << merge.weight << endl;
		mem[i - 1][k] = merge;
	}

	return min_agrupacion.weight < min_division.weight ? min_agrupacion : min_division;
}


int main () {
	vector<int> A{1, 0, 0, 1, 1, 0};
	vector<int> B{0, 1, 1, 1, 0, 0};
	cout << "Recursive Algorithm" << endl;

	Matching result_recursive = min_matching_recursive (A, B);
	cout << "Best matching :" << endl;
	for (int i = 0; i < result_recursive.matching.size (); i++)
	{
		cout << "(" << result_recursive.matching[i].i << "," << result_recursive.matching[i].j << ")" << " ";
	}
	cout<<endl;
	cout << "Minimum cost matching :" << result_recursive.weight << endl;

    cout << "Memoized Algorithm" << endl;
    Matching result_memoized = min_matching_memoized (A, B);
    cout << "Best  matching :" << endl;
    for (int i = 0; i < result_memoized.matching.size (); i++)
    {
        cout << "(" << result_memoized.matching[i].i << "," << result_memoized.matching[i].j << ")" << " ";
    }
    cout << endl;
    cout << "Minimum cost matching :" << result_memoized.weight << endl;
}
