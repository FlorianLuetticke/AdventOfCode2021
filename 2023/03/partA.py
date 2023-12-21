from collections import defaultdict
import os
source_file_dir = os.path.dirname(os.path.abspath(__file__))

with open(source_file_dir+"/"+"final.txt", "r", encoding="utf8") as f:
    schematic = []
    for line in f:
        schematic.append([s for s in line.strip()])

    print(schematic)

    numbers = []

    for i in range(len(schematic)):
        j = 0
        while j < len(schematic[i]):
            if schematic[i][j].isdigit():
                start = j
                while j < len(schematic[i]) and schematic[i][j].isdigit():
                    end = j
                    j += 1

                nrString = "".join(schematic[i][start:end+1])
                numbers.append((i, start, end, int(nrString)))

            else:
                j += 1
    print(numbers)
    sum_ = 0
    gearDict = defaultdict(list)
    for line, start, end, nr in numbers:
        minI = max(0, line-1)
        maxI = min(len(schematic)-1, line+1)
        minJ = max(0, start-1)
        maxJ = min(len(schematic[i])-1, end+1)
        for i in range(minI, maxI+1):
            for j in range(minJ, maxJ+1):
                if (not (schematic[i][j].isdigit() or schematic[i][j] == ".")):
                    print("found  symbol at ", i, j, " ",
                          schematic[i][j], " for number", line, start, end, nr)
                    sum_ += nr
                    if (schematic[i][j] == "*"):
                        gearDict[(i, j)].append(nr)
    print(gearDict)
    gearSum = 0
    for key, value in gearDict.items():
        if len(value) == 2:
            gearSum += value[0] * value[1]
    print(gearSum)
    print(sum_)
