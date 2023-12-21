from collections import defaultdict
import os
import itertools

source_file_dir = os.path.dirname(os.path.abspath(__file__))

with open(source_file_dir+"/"+"test.txt", "r", encoding="utf8") as f:

    nodeMap = {}

    for line in f:
        if line.strip() == "":
            continue
        if " = " in line:
            nodeMap[line[0:3]] = [line[7:10], line[12:15]]
        else:
            instructions = line.strip()
        cards = line.split()[0]

    print(nodeMap)
    print(instructions)

    curNode = "AAA"
    stepCount = 0
    for instruction in itertools.cycle(instructions):
        if instruction == "L":
            curNode = nodeMap[curNode][0]
        else:
            curNode = nodeMap[curNode][1]
        stepCount += 1
        print(curNode)
        if curNode == "ZZZ":
            break
    print(stepCount)

    # export graph as dot file
    with open(source_file_dir+"/"+"graph2.dot", "w", encoding="utf8") as f:
        f.write("digraph G {\n")
        for node in nodeMap:
            f.write(node+" -> "+nodeMap[node][0]+"\n")
            f.write(node+" -> "+nodeMap[node][1]+"\n")
        f.write("}")
    # run dot file through graphviz
    # dot -Tpng graph.dot -o graph.png
