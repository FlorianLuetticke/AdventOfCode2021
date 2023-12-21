from collections import defaultdict
import os


source_file_dir = os.path.dirname(os.path.abspath(__file__))


with open(source_file_dir+"/"+"final.txt", "r", encoding="utf8") as f:
    picture = []
    for line in f:
        picture.append([s for s in line.strip()])
    print("\n".join(["".join(line) for line in picture]))
    # double lines and columns only containing dots
    cost_i = [1 for i in range(len(picture))]
    cost_j = [1 for j in range(len(picture[0]))]

    for j in range(len(picture[0])):
        if (all([picture[i][j] == "." for i in range(len(picture))])):
            cost_j[j] = 1000000
    for i in range(len(picture)):
        if (all([picture[i][j] == "." for j in range(len(picture[0]))])):
            cost_i[i] = 1000000
    print(cost_i)
    print(cost_j)

    galaxyPositions = []
    for i in range(len(picture)):
        for j in range(len(picture[0])):
            if picture[i][j] == "#":
                galaxyPositions.append((i, j))

    print("\n".join(["".join(line) for line in picture]))
    print(galaxyPositions)
    distSum = 0
    for idx, (i, j) in enumerate(galaxyPositions):
        for (i2, j2) in galaxyPositions[idx+1:]:

            # print("galaxy at", i, j)
            # print("galaxy at", i2, j2)
            # print("distance", abs(i-i2)+abs(j-j2))
            minI = min(i, i2)
            maxI = max(i, i2)
            minJ = min(j, j2)
            maxJ = max(j, j2)

            distSum += sum(cost_i[minI:maxI])+sum(cost_j[minJ:maxJ])
    print(distSum)
