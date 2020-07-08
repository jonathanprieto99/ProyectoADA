#include "min_matching.h"
#include <stdlib.h>

typedef vector<Matching> trans;
typedef vector<vector<int>> matrix;

#define SIZE 4
/*Prototipos*/
trans greedy_trans (vector<vector<int>>, vector<vector<int>>);

vector<Pair> get_blocks (vector<int> A) {
        vector<Pair> blocks;
        bool flag = false;
        Pair block;
        for (int i = 0; i < A.size (); i++) {
                block.index = blocks.size ();
                if (A[i] == 1) {
                        if (!flag) {
                                flag = true;
                                block.i = i;
                        }
                        if (i == A.size () - 1) {
                                        block.j = i;
                                        blocks.push_back (block);
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

Matching greedy_matching (vector<int> A, vector<int> B) {
        Matching result;

        vector<Pair> blocks_A = get_blocks (A);
        vector<Pair> blocks_B = get_blocks (B);

        int i = 0, j = 0;
        int n = blocks_A.size ();
        int m = blocks_B.size ();
        if (n == 0 or m == 0) {
                cout << "Error: No hay bloques en uno de los vectores" << endl;
                exit (1);
        }

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
        float weight = 0;
        while (i < max && j < max) {
                match.i = blocks_A[i].index;
                match.j = blocks_B[j].index;
                weight += (blocks_A[i].j - blocks_A[i].i + 1) / (blocks_B[j].j - blocks_B[j].i + 1);
                result.matching.push_back (match);

                if (i < n)
                        i++;
                if (j < m)
                        j++;
        }
        result.weight = weight;
        return result;
}

trans greedy_trans (vector<vector<int>> A, vector<vector<int>> B) {
	trans result;
	Matching temp;
	for (int i = 0; i < A.size (); i++) {
		temp = greedy_matching (A[i], B[i]);
		result.push_back (temp);
	}	
	return result;
}

void print_matching (Matching result) {
        cout << "Matching: ";
        for (int i = 0; i < result.matching.size (); i++)
        {
                cout << "(" << result.matching[i].i << "," << result.matching[i].j << ")" << " ";
        }
        cout << endl << "Weight: " << result.weight << endl;
}

void print_trans (trans A) {
	for (int i = 0; i < A.size (); i++) {
		print_matching (A[i]);
	}
}

int main () {
	matrix A, B;
	for (int i = 0; i < SIZE; i++) {
		vector<int> temp;
		A.push_back (temp);
		B.push_back (temp);
		for (int j = 0; j < SIZE; j++) {
			A[i].push_back (rand () % 2);
			B[i].push_back (rand () % 2);
			cout << A[i][j] << " " << B[i][j] << endl;
		}
	}
	trans greedy = greedy_trans (A, B);
	print_trans (greedy);
}
