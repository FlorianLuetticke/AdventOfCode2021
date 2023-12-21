from collections import defaultdict
from functools import reduce
import os
import itertools

source_file_dir = os.path.dirname(os.path.abspath(__file__))


class Node:
    def __init__(self, name, neighbourLeft, neighbourRight):
        self.name = name
        self.neighbourLeft = neighbourLeft
        self.neighbourRight = neighbourRight

        self.parentLeft = []
        self.parentRight = []
        self.distanceToTarget = []
        self.distanceToTSelf = []
        self.reachableFromStart = set()
        self.canReachTarget = set()

    def advance(self, direction):
        if direction == "L":
            return self.neighbourLeft
        else:
            return self.neighbourRight

    def __repr__(self):
        return self.name + " -> " + self.neighbourLeft + " | " + self.neighbourRight + " parents: " + ",".join(self.parentLeft) + " | " + ",".join(self.parentRight)


def lcd(numbers):
    def gcd(a, b):
        while b:
            a, b = b, a % b
        return a

    def lcm(a, b):
        return a * b // gcd(a, b)

    return reduce(lcm, numbers, 1)


print(lcd([5, 7, 35, 2]))


with open(source_file_dir+"/"+"final.txt", "r", encoding="utf8") as f:

    nodeMap = {}

    for line in f:
        if line.strip() == "":
            continue
        if " = " in line:
            nodeMap[line[0:3]] = Node(line[0:3], line[7:10], line[12:15])
        else:
            instructions = line.strip()
        cards = line.split()[0]

    print(nodeMap)
    startNodes = [nodeMap[node] for node in nodeMap if node[-1] == "A"]
    print(startNodes)
    print(instructions)

    for name, node in nodeMap.items():

        nodeMap[node.neighbourLeft].parentLeft.append(node.name)
        nodeMap[node.neighbourRight].parentRight.append(node.name)

    curNode = [node.name for node in startNodes]

    nodeHistory = [[node] for node in curNode]
    step_count = 0
    final_step_count = None
    for instruction in itertools.cycle(instructions):
        if instruction == "L":
            curNode = [nodeMap[node].neighbourLeft for node in curNode]
        else:
            curNode = [nodeMap[node].neighbourRight for node in curNode]
        for i in range(len(curNode)):
            nodeHistory[i].append(curNode[i])

            if i == 0 and step_count < 50:
                print(nodeHistory[i])
            if curNode[i][-1] == "Z":
                # print("found target. add", nodeHistory[i])
                for dist, name in enumerate(nodeHistory[i]):
                    nodeMap[name].distanceToTarget.append(
                        len(nodeHistory[i])-dist)
                    nodeMap[name].reachableFromStart.add(startNodes[i].name)
                    nodeMap[name].canReachTarget.add(curNode[i])

        step_count += 1
        if all([len(nodeMap[node.name].distanceToTarget) > 1 for node in startNodes]):
            break
    print("worked until", step_count)
    for name, node in nodeMap.items():
        if startNodes[0].name in node.reachableFromStart:
            l = list(set(node.distanceToTarget))
            l.sort()
            print(node, l)
            continue
    multiples = []
    multiplesFull = []
    for idx in range(len(startNodes)):
        print(nodeMap[startNodes[idx].name],
              nodeMap[startNodes[idx].name].distanceToTarget)
        l = list(set(nodeMap[startNodes[idx].name].distanceToTarget))
        l.sort()

        print("InstructionLength", len(instructions), l[1]-l[0],
              "modulo len ", (l[1]-l[0]) % len(instructions), "div: ", (l[1]-l[0]) // len(instructions), "offset: ", l[1] % (l[1]-l[0]))
        multiples.append((l[1]-l[0]) // len(instructions))
        multiplesFull.append(l[1]-l[0])

        print([a-b for a, b in zip(l[1:], l[:-1])])

    mult = lcd(multiples)
    multiples.append(len(instructions))
    mult2 = lcd(multiples)
    mult3 = lcd(multiplesFull)
    print(mult)
    print(mult2)
    print("full", mult3)
    print(multiples)
    print(mult*len(instructions) - 1)
