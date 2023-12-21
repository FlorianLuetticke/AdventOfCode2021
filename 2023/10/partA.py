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
    print(np.array(distanceGrid).shape)
    np.printoptions(threshold=200)
    print(np.array(distanceGrid))
    print(max([max(x) for x in distanceGrid]))
