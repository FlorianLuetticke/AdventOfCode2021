from collections import defaultdict
import os
import numpy as np


source_file_dir = os.path.dirname(os.path.abspath(__file__))


with open(source_file_dir+"/"+"final.txt", "r", encoding="utf8") as f:
    result = 0
    resultFront = 0
    pipeGrid = []
    distanceGrid = []
    for line in f:
        pipeGrid.append([s for s in line.strip()])
        distanceGrid.append([-1 for _ in line.strip()])
    distanceGrid = np.array(distanceGrid)
    print(np.array(pipeGrid))

    def findStart():
        for i in range(len(pipeGrid)):
            for j in range(len(pipeGrid[i])):
                if pipeGrid[i][j] == "S":
                    return (i, j)
        return None
    iStart, jStart = findStart()
    print(iStart, jStart)
    distanceGrid[iStart][jStart] = 0
    np.set_printoptions(linewidth=200)

    def getNeighbours(i, j):
        if (pipeGrid[i][j] == "|"):
            return [(i-1, j), (i+1, j)]
        if (pipeGrid[i][j] == "-"):
            return [(i, j-1), (i, j+1)]
        if (pipeGrid[i][j] == "L"):
            return [(i-1, j), (i, j+1)]
        if (pipeGrid[i][j] == "J"):
            return [(i-1, j), (i, j-1)]
        if (pipeGrid[i][j] == "7"):
            return [(i+1, j), (i, j-1)]
        if (pipeGrid[i][j] == "F"):
            return [(i+1, j), (i, j+1)]
        if (pipeGrid[i][j] == "S"):
            return [(i+1, j), (i, j+1), (i-1, j), (i, j-1)]
        if (pipeGrid[i][j] == "."):
            return []
        raise Exception("Unknown pipe type " +
                        pipeGrid[i][j] + " at " + str(i) + " " + str(j))

    # Breadth first search

    queue = [(iStart, jStart)]

    while (len(queue) > 0):
        i, j = queue.pop(0)
        for iNeighbour, jNeighbour in getNeighbours(i, j):
            if (iNeighbour < 0 or iNeighbour >= len(pipeGrid) or jNeighbour < 0 or jNeighbour >= len(pipeGrid[iNeighbour])):
                continue
            validNeighbours = getNeighbours(iNeighbour, jNeighbour)
            if (i, j) not in validNeighbours:
                print("Invalid neighbour", (i, j), "at", (iNeighbour, jNeighbour),
                      "valid", validNeighbours, "source Neighbours", getNeighbours(i, j))
                continue

            if (distanceGrid[iNeighbour][jNeighbour] == -1 or distanceGrid[iNeighbour][jNeighbour] > distanceGrid[i][j] + 1):
                distanceGrid[iNeighbour][jNeighbour] = distanceGrid[i][j] + 1
                queue.append((iNeighbour, jNeighbour))
    print(distanceGrid.shape)
    np.printoptions(threshold=200)

    endPoint = np.unravel_index(np.argmax(distanceGrid), distanceGrid.shape)

    print(endPoint)

    # create a clean grid

    cleanGrid = [["." for _ in range(len(pipeGrid[0]))]
                 for _ in range(len(pipeGrid))]
    queue = [endPoint]
    while (len(queue) > 0):
        i, j = queue.pop(0)
        cleanGrid[i][j] = pipeGrid[i][j]
        if pipeGrid[i][j] == "S":
            print("Found start - do nothing")
            continue
        for iNeighbour, jNeighbour in getNeighbours(i, j):

            if (iNeighbour < 0 or iNeighbour >= len(pipeGrid) or jNeighbour < 0 or jNeighbour >= len(pipeGrid[iNeighbour])):
                continue
            validNeighbours = getNeighbours(iNeighbour, jNeighbour)
            if (i, j) not in validNeighbours:
                print("Invalid neighbour", (i, j), "at", (iNeighbour, jNeighbour),
                      "valid", validNeighbours, "source Neighbours", getNeighbours(i, j))
                continue
            if (cleanGrid[iNeighbour][jNeighbour] == "."):
                queue.append((iNeighbour, jNeighbour))
            cleanGrid[iNeighbour][jNeighbour] = pipeGrid[iNeighbour][jNeighbour]
    print(np.array(cleanGrid))
    print(iStart, jStart)
    print(np.array(cleanGrid)[iStart-5:iStart+5, jStart-5:jStart+5])
    cleanGrid[iStart][jStart] = "|"
    print(np.array(cleanGrid)[iStart-5:iStart+5, jStart-5:jStart+5])
    print(max([max(x) for x in distanceGrid]))

    corners = ["L", "J", "7", "F"]
    verticalOpposites = [("L", "7"), ("F", "J")]
    horizontalOpposites = [("7", "L"), ("F", "J")]
    for i in range(len(cleanGrid)):
        for j in range(len(cleanGrid[i])):
            if (cleanGrid[i][j] == "."):
                verticalCount = 0
                horizontalCount = 0
                cur_i = 0
                while cur_i < i:
                    if cleanGrid[cur_i][j] == "-":
                        horizontalCount += 1

                    if cleanGrid[cur_i][j] in corners:
                        start_i = cur_i
                        cur_i += 1
                        while cur_i < i:
                            if cleanGrid[cur_i][j] in corners:
                                if (cleanGrid[start_i][j],  cleanGrid[cur_i][j]) in horizontalOpposites:
                                    horizontalCount += 1
                                cur_i += 1
                                break
                            cur_i += 1
                        continue

                    cur_i += 1

                cur_j = 0
                while cur_j < j:
                    if cleanGrid[i][cur_j] == "|":
                        verticalCount += 1

                    if cleanGrid[i][cur_j] in corners:
                        start_j = cur_j
                        cur_j += 1
                        while cur_j < j:
                            if cleanGrid[i][cur_j] in corners:
                                if (cleanGrid[i][start_j],  cleanGrid[i][cur_j]) in verticalOpposites:
                                    verticalCount += 1
                                cur_j += 1
                                break
                            cur_j += 1
                        continue

                    cur_j += 1

                if (verticalCount % 2 == 1 and horizontalCount % 2 == 1):
                    cleanGrid[i][j] = "x"
                else:
                    cleanGrid[i][j] = " "
    print(np.array(cleanGrid))
    print("\n".join(["".join(x) for x in pipeGrid]))
    print("\n".join(["".join(x) for x in cleanGrid]))
    print("inside", np.count_nonzero(np.array(cleanGrid) == 'x'))
