from collections import defaultdict
import os


source_file_dir = os.path.dirname(os.path.abspath(__file__))


with open(source_file_dir+"/"+"final.txt", "r", encoding="utf8") as f:
    result = 0
    resultFront = 0
    for line in f:
        sequences = [list(map(int, line.split()))]
        while True:
            sequences.append(
                [a-b for a, b in zip(sequences[-1][1:], sequences[-1][:-1])])
            if (all([x == 0 for x in sequences[-1]])):
                break
        sequences.reverse()
        print(sequences)
        for i in range(len(sequences)-1):
            sequences[i+1].append(sequences[i+1][-1] + sequences[i][-1])
            sequences[i+1] = [sequences[i+1][0] -
                              sequences[i][0]]+sequences[i+1]
        result += sequences[-1][-1]
        resultFront += sequences[-1][0]
        print(sequences)
    print(result)
    print(resultFront)
