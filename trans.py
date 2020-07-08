from dataclasses import dataclass
import random

@dataclass 
class Pair:
    i: float
    j: float
    index: int

@dataclass
class Matching:
    matching: list
    weight: float

def get_blocks (A):
    blocks = []
    flag = False
    block = Pair (0, 0, 0)
    aux_blocks = []
    j = 0
    for i in range (len (A)):
        aux_blocks.append (Pair (0, 0, 0))
        block.index = len (blocks)
        if A[i] == 1:
            if not flag:
                flag = True
                block.i = i
            if i == len (A) - 1:
                block.j = i
                aux_blocks[j].i = block.i
                aux_blocks[j].j = block.j
                aux_blocks[j].index = block.index
                blocks.append (aux_blocks[j])
                j += 1
        elif A[i] == 0:
            if flag:
                flag = False
                block.j = i - 1
                aux_blocks[j].i = block.i
                aux_blocks[j].j = block.j
                aux_blocks[j].index = block.index
                blocks.append (aux_blocks[j])
                j += 1

    return blocks

def greedy_matching (A, B):
    result = Matching([], 0)

    blocks_A = get_blocks (A)
    blocks_B = get_blocks (B)
    i = 0
    j = 0
    n = len (blocks_A)
    m = len (blocks_B)

    if n == 0 or m == 0:
        print ("Error: No hay bloques en uno de los vectores")
    
    max_value = 0
    if n > m:
        max_value = n
        m -= 1
    else:
        max_value = m

    match = Pair (0, 0, 0)
    weight = 0
    aux_matches = []
    k = 0
    while i < max_value and j < max_value:
        aux_matches.append (Pair (0, 0, 0))
        match.i = blocks_A[i].index
        match.j = blocks_B[j].index
        weight += (blocks_A[i].j - blocks_A[i].i + 1) / (blocks_B[j].j - blocks_B[j].i + 1)
        aux_matches[k].i = match.i
        aux_matches[k].j = match.j
        result.matching.append (aux_matches[k])
        k += 1
        if i < n:
            i += 1
        if j < m:
            j += 1

        result.weight = weight
    return result

def greedy_trans (A, B):
    result = []
    temp = Matching ([], 0)
    for i in range (len (A)):
        temp = greedy_matching (A[i], B[i])
        result.append (temp)
    return result

size = 4
A = []
B = []

for i in range (size):
    temp = []
    A.append (temp)
    B.append (temp)
    for j in range (size):
        A[i].append (random.randint (0, 1))
        B[i].append (random.randint (0, 1))

for i in range (size):
    print (A[i])
    print (B[i])

trans_greedy = greedy_trans (A, B)

for i in range (len (trans_greedy)):
    print (trans_greedy[i])
