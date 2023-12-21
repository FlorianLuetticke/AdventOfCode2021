from collections import defaultdict
import os


source_file_dir = os.path.dirname(os.path.abspath(__file__))


with open(source_file_dir+"/"+"final.txt", "r", encoding="utf8") as f:
    patterns = []
    missingCount = []
    block_sizes = []
    for line in f:
        print(line)
        block_sizes.append([int(s) for s in line.split()[1].split(",")])
        patterns.append([s for s in line.split()[0]])
        missingCount.append(patterns[-1].count("?"))

    print(patterns)
    print(block_sizes)
    print(missingCount)
    total = 0
    for i in range(len(patterns)):

        # create possible patterns by replacing ? with . and #
        # and then check if they match the block sizes
        for count in range(2**missingCount[i]):
            curPattern = patterns[i].copy()

            for j in range(len(curPattern)):
                if curPattern[j] == "?":
                    curPattern[j] = "." if count % 2 == 0 else "#"
                    count //= 2

            res = list(filter(None, "".join(curPattern).split(".")))
            if len(res) != len(block_sizes[i]):
                continue
            if all([len(res[j]) == block_sizes[i][j] for j in range(len(res))]):
                print("found pattern", curPattern,
                      "for", i, "with", block_sizes[i])
                total += 1
    print(total)
