#include <iostream>
#include <vector>

using namespace std;

struct Pair {
	int i;
	int j;
};

/* Prototipos */
vector<Pair> get_blocks (vector<int>);
vector<Pair> greedy_matching (vector<int>, vector<int>);


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
/*

vector<Pair> greedy_matching_recursive (vector<int> A, vector<int> B) {
    vector<Pair> result;

    vector<Pair> blocks_A = get_blocks (A);
    vector<Pair> blocks_B = get_blocks (B);

    int i = 0, j = 0, k = 0;
    int n = blocks_A.size ();
    int m = blocks_B.size ();

    int max;

    return result;
}
 */

int main () {
	vector<int> A{1, 0, 0, 1, 1, 0, 1};
	vector<int> B{0, 1, 1, 1, 0, 0, 1};

	vector<Pair> result = greedy_matching (A, B);
	for (int i = 0; i < result.size (); i++)
	{
		cout << "(" << result[i].i << "," << result[i].j << ")" << " ";
	}
	cout << endl;
}
