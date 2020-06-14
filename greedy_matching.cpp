#include <iostream>
#include <vector>
#include <climits>

using namespace std;

struct Pair {
	float i;
	float j;
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
			}
			if (i == A.size () - 1)	{
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
	float weight = 0;
	while (i < max && j < max) {
		/*if (i == n) {
                        float sum = 0;
                        while (j < max) {
				match.i = blocks_A[i].i;
				match.j = blocks_B[j].j;
				result.matching.push_back (match);
                                sum += blocks_B[j].j - blocks_B[j].i + 1;
                                j++;
                        }
                        weight += (blocks_A[i].j - blocks_A[i].i + 1) / sum;
                }
                else if (j == m) {
                        float sum = 0;
                        while (i < max) {
                                match.i = blocks_A[i].i;
                                match.j = blocks_B[j].j;
                                result.matching.push_back (match);
                                sum += blocks_A[i].j - blocks_A[i].i + 1;
                                i++;
                        }
                        weight += sum / (blocks_B[j].j - blocks_B[j].i + 1);
		}*/
		/*else {*/
			match.i = blocks_A[i].i;
			match.j = blocks_B[j].j;
			weight += (blocks_A[i].j - blocks_A[i].i + 1) / (blocks_B[j].j - blocks_B[j].i + 1);
			result.matching.push_back (match);

			if (i < n)
				i++;
			if (j < m)
				j++;	
		/*}*/
	}
	result.weight = weight;
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

	min_agrupacion.weight = INT_MAX;
	min_division.weight = INT_MAX;

	if (A.size () == 1 and B.size () == 1) {
		Pair match;
		match.i = A[0].i;
		match.j = B[0].j;
		min_match.matching.push_back (match);
		min_match.weight = (A[0].j - A[0].i + 1) / (B[0].j - B[0].i + 1);
		return min_match;
	}
		
	if (A.size () == 1 and B.size () > 1) {
		Pair match;
		float sum = 0;
		match.i = A[0].i;
		for (int it = 0; it < B.size (); it++) {
			match.j = B[it].j;
			min_match.matching.push_back (match);
			sum += B[it].j - B[it].i + 1;
		}
		min_match.weight = (A[0].j - A[0].i + 1) / sum;
		return min_match;
	}
	if (B.size () == 1 and A.size () > 1){
		Pair match;
		float sum = 0;
		match.j = B[0].j;
		for (int it = 0; it < A.size (); it++){
			match.i = A[it].i;
			min_match.matching.push_back (match);
			sum += A[it].j - A[it].i + 1;
		}
		min_match.weight = sum / (B[0].j - B[0].i + 1);
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
		print_matching (merge);
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
		print_matching (merge);
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

    Matching min_matching = opt_solution_mem (blocks_A, blocks_B, false);

    return min_matching;
}

Matching opt_solution_mem (vector<Pair> A, vector<Pair> B, bool left) {
	Matching min_match;
	int i = A.size () - 1;
	int j = B.size () - 1;
	Matching min_agrupacion;
	Matching min_division;

	min_agrupacion.weight = INT_MAX;
	min_division.weight = INT_MAX;
	if (left) {
		if (A.size () <= mem.size () - 1 and B.size () <= mem[0].size () - 1) {	
			if (mem[A.size ()][B.size ()].weight != 0) {
				return mem[A.size ()][B.size ()];
			}
		}
	}
	if (A.size () == 1 and B.size () == 1) {
		Pair match;
                match.i = A[0].i;
                match.j = B[0].j;
                min_match.matching.push_back (match);
                min_match.weight = (A[0].j - A[0].i + 1) / (B[0].j - B[0].i + 1);
                return min_match;
	}
		
	if (A.size () == 1 and B.size () > 1) {
		Pair match;
                float sum = 0;
                match.i = A[0].i;
                for (int it = 0; it < B.size (); it++) {
                        match.j = B[it].j;
                        min_match.matching.push_back (match);
                        sum += B[it].j - B[it].i + 1;
                }
                min_match.weight = (A[0].j - A[0].i + 1) / sum;
                return min_match;
	}

	if (B.size () == 1 and A.size () > 1){
		Pair match;
                float sum = 0;
                match.j = B[0].j;
                for (int it = 0; it < A.size (); it++){
                        match.i = A[it].i;
                        min_match.matching.push_back (match);
                        sum += A[it].j - A[it].i + 1;
                }
                min_match.weight = sum / (B[0].j - B[0].i + 1);
                return min_match;
	}
	
	for (int k = i - 1; k >= 0; k--) {
		vector<Pair> left_A (A.begin (), A.begin () + k + 1);
		vector<Pair> right_A (A.begin () + k + 1, A.end ());
		
		vector<Pair> left_B (B.begin (), B.end () - 1);
		vector<Pair> right_B; right_B.push_back (B[B.size () - 1]);
		
		Matching min_left = opt_solution_mem (left_A, left_B, true);
		Matching min_right = opt_solution_mem (right_A, right_B, false);
		
		Matching merge;
		merge.matching = merge_matchings (min_left.matching, min_right.matching);
		merge.weight = min_left.weight + min_right.weight;
		if (merge.weight < min_agrupacion.weight)
			min_agrupacion = merge;
		/*for (int h = 0; h < merge.matching.size (); h++)
        	{
                	cout << "(" << merge.matching[h].i << "," << merge.matching[h].j << ")" << " ";
        	}
		cout << merge.weight << endl;*/
		print_matching (merge);
		mem[left_A.size ()][left_B.size ()] = min_left;
	}
	for (int k = j - 1; k >= 0; k--) {
		vector<Pair> left_A (A.begin (), A.end () - 1);
		vector<Pair> right_A; right_A.push_back (A[A.size () - 1]);

		vector<Pair> left_B (B.begin (), B.begin () + k + 1);
		vector<Pair> right_B (B.begin () + k + 1, B.end ());
		
		Matching min_left = opt_solution_mem (left_A, left_B, true);
		Matching min_right = opt_solution_mem (right_A, right_B, false);
		
		Matching merge;
		merge.matching = merge_matchings (min_left.matching, min_right.matching);
		merge.weight = min_left.weight + min_right.weight;
		if (merge.weight < min_division.weight)
			min_division = merge;
		/*for (int h = 0; h < merge.matching.size (); h++)
        	{
                	cout << "(" << merge.matching[h].i << "," << merge.matching[h].j << ")" << " ";
        	}
		cout << merge.weight << endl;*/
		print_matching (merge);
		mem[left_A.size ()][left_B.size ()] = min_left;
	}

	return min_agrupacion.weight < min_division.weight ? min_agrupacion : min_division;
}

void print_matching (Matching result) {
	cout << "Matching: ";
        for (int i = 0; i < result.matching.size (); i++)
        {
                cout << "(" << result.matching[i].i << "," << result.matching[i].j << ")" << " ";
        }
        cout << endl << "Weight: " << result.weight << endl;
}

int main () {
	vector<int> A{1, 0, 0, 0, 0, 1, 1, 0, 1};
	vector<int> B{1, 1, 0, 0, 0, 0, 0, 1, 1};

	/*while (true) {
		string str_A, str_B;
		cout << "Ingrese arreglo A: ";
		cin >> str_A;
		cout << "Ingrese Arreglo B: ";
		cin >> str_B;
		if (A.length() == B.length ()) {
			vector<int> A = string_to_vector (str_A);
        		vector<int> B = string_to_vector (str_B);
			if (valid_vector (A) and valid_vector (B))
				break;
		}
	}*/
	
	Matching result;
	int option;
	cout << "[1] Matching Voraz" << endl;
	cout << "[2] Matching Minimo Recursivo" << endl;
	cout << "[3] Matching Minimo Memoizado" << endl;
	cout << "[0] Salir" << endl;
	while (true) {
		cout << "Ingrese el algoritmo a utilizar: ";
		cin >> option;
		switch (option) {
			case 0:
				return 0;
			case 1: ;
				result = greedy_matching (A, B);
				cout << "resultado" << endl;
				print_matching (result);
				break;
			case 2: ;
				result = min_matching_recursive (A, B);
				cout << "resultado" << endl;
				print_matching (result);
				break;
			case 3: ;
				result = min_matching_memoized (A, B);
				cout << "resultado" << endl;
				print_matching (result);
				break;
			default:
				cout << "No existe la opcion " << option << endl;

		}
	}
}
