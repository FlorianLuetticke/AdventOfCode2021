from collections import defaultdict
import os


source_file_dir = os.path.dirname(os.path.abspath(__file__))


def find_in(l, key):
    for idx in range(len(l)):
        if l[idx][0] == key:
            return idx
    return None


with open(source_file_dir+"/"+"final.txt", "r", encoding="utf8") as f:

    grid = []
    for line in f:
        line_new = line.replace("\n", "")
        grid = line_new.split(",")
        sum_val = 0
        box_commands = defaultdict(list)
        for val in grid:

            value = 0
            idx = 0
            for s in val:
                if s in ["-", "="]:
                    break
                value += ord(s)
                value *= 17
                value = value % 256
                idx += 1
            print("Input:", val, "Output:", value)
            sum_val += value
            box_commands[value].append((val[:idx], val[idx:].replace("=", "")))

        box_state = defaultdict(list)
        for i in range(256):
            if i in box_commands:
                print(i, box_commands[i])
                for command in box_commands[i]:
                    pos = find_in(box_state[i], command[0])
                    if command[1] == "-":
                        if pos is not None:
                            box_state[i].pop(pos)
                    else:
                        if pos is None:
                            box_state[i].append(command)
                        else:
                            box_state[i][pos] = command
                print(box_state)
        box_sum = 0

        for i in range(256):
            if i in box_state:
                for j in range(len(box_state[i])):

                    box_sum += (i+1)*(j+1)*int(box_state[i][j][1])
        print("Box Sum:", box_sum)
        print("Sum:", sum_val)
