from collections import defaultdict
import os


source_file_dir = os.path.dirname(os.path.abspath(__file__))


with open(source_file_dir+"/"+"final.txt", "r", encoding="utf8") as f:
    picture = []
    for line in f:
        picture.append([s for s in line.strip()])
    print("\n".join(["".join(line) for line in picture]))
    # double lines and columns only containing dots
    add_j = []
    for j in range(len(picture[0])):
        if (all([picture[i][j] == "." for i in range(len(picture))])):
            add_j.append(j)
    add_j.reverse()
    print(add_j)
    for j in add_j:
        for i in range(len(picture)):
            picture[i].insert(j, ".")
    add_i = []
    for i in range(len(picture)):
        if (all([picture[i][j] == "." for j in range(len(picture[0]))])):
            add_i.append(i)
    add_i.reverse()
    print(add_i)
    for i in add_i:
        picture.insert(i, picture[i])

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
            distSum += abs(i-i2)+abs(j-j2)
    print(distSum)
