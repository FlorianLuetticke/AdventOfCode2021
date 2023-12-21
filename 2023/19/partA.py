from collections import defaultdict
import os


source_file_dir = os.path.dirname(os.path.abspath(__file__))


next_pix = {".": {"left": ["left"], "right": ["right"], "up": ["up"], "down": ["down"]}, "|": {"left": ["up", "down"], "right": [
    "up", "down"], "up": ["up"], "down": ["down"]}, "-": {"left": ["left"], "right": ["right"], "up": ["left", "right"], "down": ["left", "right"]}, "/": {"left": ["down"], "right": ["up"], "up": ["right"], "down": ["left"]},
    "\\": {"left": ["up"], "right": ["down"], "up": ["left"], "down": ["right"]}}


def next_pos(pos, direction):
    if direction == "left":
        return (pos[0], pos[1]-1)
    elif direction == "right":
        return (pos[0], pos[1]+1)
    elif direction == "up":
        return (pos[0]-1, pos[1])
    elif direction == "down":
        return (pos[0]+1, pos[1])


rule_dict = {}
part_list = []
with open(source_file_dir+"/"+"final.txt", "r", encoding="utf8") as f:

    grid = []
    energized = []
    read_rules = True
    for line in f:
        print(line.strip())
        if read_rules:
            if line.strip() == "":
                read_rules = False
                continue
            rule_name = line.split("{")[0]
            rule_string = line.split("{")[1].split("}")[0]
            rule_dict[rule_name] = []
            for r_raw in rule_string.split(","):
                split = r_raw.split(":")
                print(split)
                condition = None
                if len(split) == 2:

                    if "<" in split[0]:
                        cond = split[0].split("<")
                        condition = (cond[0], "<", int(cond[1]))
                    elif ">" in split[0]:
                        cond = split[0].split(">")
                        condition = (cond[0], ">", int(cond[1]))
                    else:
                        raise Exception("Invalid condition")
                rule_dict[rule_name].append((condition, split[-1]))
        else:
            part_string = line.strip()[1:-1].split(",")
            part_list.append({s.split("=")[0]: int(
                s.split("=")[1]) for s in part_string})

    print(rule_dict)
    print(part_list)

    accepted = False
    accepted_sum = 0

    for part in part_list:
        visited = {s: False for s in rule_dict.keys()}
        current = "in"

        while True:
            print(current, part)
            if current == "A":
                accepted = True
                break
            if current == "R":
                accepted = False
                break
            if visited[current]:
                print("Loop detected")
                break
            visited[current] = True

            for rule in rule_dict[current]:
                if rule[0] is None:
                    current = rule[1]
                    break
                else:
                    if rule[0][1] == "<":
                        if part[rule[0][0]] < rule[0][2]:
                            current = rule[1]
                            break
                    elif rule[0][1] == ">":
                        if part[rule[0][0]] > rule[0][2]:
                            current = rule[1]
                            break
        for k, v in part.items():
            if accepted:
                accepted_sum += v
            print(k, v)
print(accepted_sum)
