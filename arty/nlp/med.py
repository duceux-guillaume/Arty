#!/usr/bin/env python3

def levenshtein(s1, s2):
    if len(s1) > len(s2):
        return levenshtein(s2, s1)

    if len(s1) == 0:
        return len(s2) # number of insertion
    previous = range(len(s1) + 1)
    for i, c2 in enumerate(s2):
        current = [i + 1]
        for j, c1 in enumerate(s1):
            insertions = previous[j + 1] + 1 
            deletions = current[j] + 1
            substitutions = previous[j] + (c1 != c2)*2
            current.append(min(insertions, deletions, substitutions))
        previous = current
    return previous[-1]

if __name__ == "__main__":
    import os
    import sys
    filename = os.path.dirname(__file__)
    print(filename)
    f = open(filename + "/../../dataset/keywords.txt")
    dataset = f.read().splitlines()
    print(dataset)
    print(sys.argv)
    if len(sys.argv) >= 2:
        for keyw in dataset:
            print([keyw, sys.argv[1], levenshtein(keyw, sys.argv[1])])
