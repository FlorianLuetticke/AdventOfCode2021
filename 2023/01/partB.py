import os


replace = {"one": "1", "two": "2", "three": "3", "four": "4",
           "five": "5", "six": "6", "seven": "7", "eight": "8", "nine": "9"}


def replace_all(line):
    while True:
        minKey = None
        curIdx = -1
        for key, value in replace.items():
            idx = line.find(key)
            if idx != -1 and (idx < curIdx or curIdx == -1):
                if idx != -1 and curIdx != -1:
                    if (idx+len(key) > curIdx and idx <= curIdx) or (curIdx+len(minKey) > idx and curIdx <= idx):
                        print("Overlapping: ", key,
                              minKey, "at ", idx, curIdx)
                        print(" in line: ", line.strip())
                minKey = key
                curIdx = idx
        if minKey != None:
            print("input", line, "minKey", minKey, "curIdx", curIdx,
                  line[:curIdx], replace[minKey], line[curIdx+1:])
            # if (curIdx == 0):
            #     line = replace[minKey] + line[curIdx+1:]
            # else:
            line = line[:curIdx] + replace[minKey] + line[curIdx+1:]
            print("Result", line)

        else:
            break
    return line


def result(filename):
    source_file_dir = os.path.dirname(os.path.abspath(__file__))
    n = 0
    with open(source_file_dir+"/"+filename, "r", encoding="utf8") as f:
        for line in f:
            # print(line.strip())
            while True:
                minKey = None
                curIdx = -1
                for key, value in replace.items():
                    idx = line.find(key)
                    if idx != -1 and (idx < curIdx or curIdx == -1):
                        if idx != -1 and curIdx != -1:
                            if (idx+len(key) > curIdx and idx <= curIdx) or (curIdx+len(minKey) > idx and curIdx <= idx):
                                print("Overlapping: ", key,
                                      minKey, "at ", idx, curIdx)
                                print(" in line: ", line.strip())
                        minKey = key
                        curIdx = idx
                if minKey != None:
                    print("input", line, "minKey", minKey, "curIdx", curIdx,
                          line[:curIdx], replace[minKey], line[curIdx+1:])
                    # if (curIdx == 0):
                    #     line = replace[minKey] + line[curIdx+1:]
                    # else:
                    line = line[:curIdx] + replace[minKey] + line[curIdx+1:]
                    print("Result", line)

                else:
                    break
            # print(line.strip())
            # extract all digits from the line
            v = [int(s) for s in line if s.isdigit()]
            print(v[0]*10 + v[-1])
            n += v[0]*10 + v[-1]

    print(n)


result("test.txt")
result("final.txt")


print(replace_all("vqhvfxrxhpdgqhcrrczjlmhdnlz7tvtrrktd9twonenqn"))
