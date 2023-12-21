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
with open(source_file_dir+"/"+"final.txt", "r", encoding="utf8") as f:

    read_rules = True
    for line in f:
        print(line.strip())
        if read_rules:
            if line.strip() == "":
                read_rules = False
                break
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


print(rule_dict)

part_repr = {"x": (1, 4000), "m": (1, 4000), "a": (1, 4000), "s": (1, 4000)}

work_queue = [(part_repr, "in", [])]

accepted_list = []


def split_parts(rule_, inp):
    if rule_[0] is None:
        return [inp.copy(), None]

    if rule_[0][1] == "<":

        cur_min, cur_max = inp[rule_[0][0]]

        if cur_max < rule_[0][2]:
            return [inp.copy(), None]
        if cur_min >= rule_[0][2]:
            return [None, inp.copy()]
        new_inp = inp.copy()
        new_inp[rule_[0][0]] = (cur_min, rule_[0][2]-1)
        new_remaining = inp.copy()
        new_remaining[rule_[0][0]] = (rule_[0][2], cur_max)
        return [new_inp, new_remaining]
    elif rule_[0][1] == ">":
        cur_min, cur_max = inp[rule_[0][0]]
        if cur_min > rule_[0][2]:
            return [inp.copy(), None]
        if cur_max <= rule_[0][2]:
            return [None, inp.copy()]
        new_inp = inp.copy()
        new_inp[rule_[0][0]] = (rule_[0][2]+1, cur_max)
        new_remaining = inp.copy()
        new_remaining[rule_[0][0]] = (cur_min, rule_[0][2])
        return [new_inp, new_remaining]


while len(work_queue) > 0:
    part, current, visited = work_queue.pop(0)

    print(current, part)
    if current == "A":
        accepted_list.append(part)
        continue
    if current == "R":
        print("rejected", part)
        continue
    if current in visited:
        print("loop")
    visited.append(current)
    for rule in rule_dict[current]:

        accepted, remaining = split_parts(rule, part)
        if accepted is not None:
            print("accepted for", rule[1], accepted)
            work_queue.append((accepted, rule[1], visited.copy()))
        if remaining is not None:
            part = remaining
        else:
            break


print(accepted_list)
score = 0
for a in accepted_list:
    mult_score = 1
    for k, v in a.items():

        mult_score *= v[1]-v[0]+1
    score += mult_score
print(score)
