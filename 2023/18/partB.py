from collections import defaultdict
import os
import heapq
from copy import deepcopy

source_file_dir = os.path.dirname(os.path.abspath(__file__))

directions_map = {"0": "R", "1": "D", "2": "L", "3": "U"}
commands = []
with open(source_file_dir+"/"+"final.txt", "r", encoding="utf8") as f:
    for line in f:
        hexCode = line.split()[2][2:-1]
        direction = directions_map[hexCode[-1]]
        length = int(hexCode[:-1], 16)

        commands.append((direction, length))

print(commands)


def next_position(pos, direction, length):
    if direction == "L":
        return (pos[0], pos[1]-length)
    elif direction == "R":
        return (pos[0], pos[1]+length)
    elif direction == "U":
        return (pos[0]-length, pos[1])
    elif direction == "D":
        return (pos[0]+length, pos[1])

# create edges directly


edges_with_direction_and_inside = []

cur_pos = (0, 0)
pos_list = [cur_pos]
for command in commands:
    next_pos = next_position(cur_pos, command[0], command[1])
    pos_list.append(next_pos)
    edges_with_direction_and_inside.append(
        [cur_pos, next_pos, command[0], "?", command[1] if command[0] in ["D", "R"] else -command[1]])
    cur_pos = next_pos
print(pos_list)


# get min/max x/y
min_x = min([pos[0] for pos in pos_list])
max_x = max([pos[0] for pos in pos_list])
min_y = min([pos[1] for pos in pos_list])
max_y = max([pos[1] for pos in pos_list])
print(min_x, max_x, min_y, max_y)
for el in edges_with_direction_and_inside:
    el[0] = (el[0][0]-min_x, el[0][1]-min_y)
    el[1] = (el[1][0]-min_x, el[1][1]-min_y)

max_x -= min_x
min_x = 0
max_y -= min_y
min_y = 0

for el in edges_with_direction_and_inside:

    if el[0][0] == min_x and el[1][0] == min_x:
        print("found", el[0], el[1])
        el[3] = "D"
        break

print(edges_with_direction_and_inside)


opposite = {"U": "D", "D": "U", "L": "R", "R": "L"}


def determine_dir(edge1, edge2):
    if edge1[3] == "?" or edge2[3] != "?":
        return
    cur_inside = edge1[3]
    cur_direction = edge1[2]
    next_direction = edge2[2]

    if cur_inside == next_direction:
        edge2[3] = opposite[cur_direction]
    else:
        edge2[3] = cur_direction


# determine the inside direction
for count in range(2):
    for i in range(len(edges_with_direction_and_inside)-1):
        determine_dir(edges_with_direction_and_inside[i],
                      edges_with_direction_and_inside[i+1])

    determine_dir(
        edges_with_direction_and_inside[-1], edges_with_direction_and_inside[0])

print(edges_with_direction_and_inside)

print_map = {"U": "^", "D": "v", "L": "<", "R": ">"}


def first_is_between(val1, val2, val3):
    return min(val3, val2) <= val1 <= max(val3, val2) and not (val1 == val2 and val1 == val3)


def edges_cross(edge1, edge2):
    if edge1[2] == edge2[2] or edge1[2] == opposite[edge2[2]]:
        return False
    if edge1[1] == edge2[0] or edge1[0] == edge2[1]:
        return False
    if edge1[2] == "U" or edge1[2] == "D":
        return first_is_between(edge1[0][1], edge2[0][1], edge2[1][1]) and first_is_between(edge2[0][0], edge1[0][0], edge1[1][0])
    else:
        return first_is_between(edge1[0][0], edge2[0][0], edge2[1][0]) and first_is_between(edge2[0][1], edge1[0][1], edge1[1][1])


def create_reduced_grid(edges):

    # later part is for printing incomplete subgrids.
    x_set = {edge[0][0] for edge in edges} | {edge[1][0] for edge in edges}
    y_set = {edge[0][1] for edge in edges} | {edge[1][1] for edge in edges}

    x_l = list(x_set)
    y_l = list(y_set)
    x_l.sort()
    y_l.sort()
    factor = 2 if max(len(x_l), len(y_l)) < 100 else 1
    x_dict = {val: factor*idx for idx, val in enumerate(x_l)}
    y_dict = {val: factor*idx for idx, val in enumerate(y_l)}
    reduced = []
    for edge in edges:
        if edge[2] in ["L", "R"]:
            reduced.append([
                (x_dict[edge[0][0]], y_dict[edge[0][1]]),
                (x_dict[edge[1][0]], y_dict[edge[1][1]]),
                edge[2], edge[3], y_dict[edge[1][1]]-y_dict[edge[0][1]]])
        else:
            reduced.append([
                (x_dict[edge[0][0]], y_dict[edge[0][1]]),
                (x_dict[edge[1][0]], y_dict[edge[1][1]]),
                edge[2], edge[3], x_dict[edge[1][0]]-x_dict[edge[0][0]]])
    return reduced


allow_print = False
# edges_with_direction_and_inside = create_reduced_grid(
#     edges_with_direction_and_inside)
allow_print = True


def print_subgrid(edges):
    if not allow_print:
        return
    use_edges = create_reduced_grid(edges)
    min_x_ = min([edge[0][0] for edge in use_edges] + [edge[1][0]
                 for edge in use_edges])
    max_x_ = max([edge[0][0] for edge in use_edges] + [edge[1][0]
                 for edge in use_edges])
    min_y_ = min([edge[0][1] for edge in use_edges] + [edge[1][1]
                 for edge in use_edges])
    max_y_ = max([edge[0][1] for edge in use_edges] + [edge[1][1]
                 for edge in use_edges])
    grid_ = [["." for _ in range(max_y_-min_y_+1)]
             for _ in range(max_x_-min_x_+1)]

    for edge in use_edges:
        start_ = edge[0]
        end_ = edge[1]
        for i in range(min(start_[0], end_[0]), max(start_[0], end_[0])+1):
            for j in range(min(start_[1], end_[1]), max(start_[1], end_[1])+1):

                grid_[i-min_x_][j-min_y_] = print_map[edge[3]]

    print("\n".join(["".join(map(str, l)) for l in grid_]))


reduce_step = 0
sum_cut = 0
print_subgrid(edges_with_direction_and_inside)
# simple_count = [count_simple(edges_with_direction_and_inside)]
while len(edges_with_direction_and_inside) > 4:
    reduce_step += 1

    cutFound = False
    delete = []
    modified = [False for _ in range(len(edges_with_direction_and_inside))]
    for i in range(len(edges_with_direction_and_inside)):
        indices = [(i + j) % len(edges_with_direction_and_inside)
                   for j in range(-2, 3)]
        indices_exp = [(i + j) % len(edges_with_direction_and_inside)
                       for j in range(-3, 4)]
        prev_prev = edges_with_direction_and_inside[indices[0]]
        prev_edge = edges_with_direction_and_inside[indices[1]]
        center = edges_with_direction_and_inside[indices[2]]
        next_edge = edges_with_direction_and_inside[indices[3]]
        next_next = edges_with_direction_and_inside[indices[4]]
        backup = [deepcopy(edges_with_direction_and_inside[j])
                  for j in indices]
        if any(modified[i] for i in indices):
            continue
        if prev_edge[2] != opposite[next_edge[2]]:
            # we want half a block, not a zigzag
            continue
        if prev_edge[2] == center[3] or next_edge[2] == opposite[center[3]]:
            # we want to cut blocks going outside
            continue
        # cut: Either perv or next will vanish.
        modified_edge = None
        modified_idx = None
        shrink = None
        delete_candidate = []
        if (abs(prev_edge[4]) == abs(next_edge[4])):
            # prev_edge  and next_edge will vanish.
            # check if we create a there and back edge
            if prev_prev[2] != center[2] or next_next[2] != center[2]:
                continue
            print(" Special case!", prev_prev, prev_edge,
                  center, next_edge, next_next, "change pos", next_edge[0], "by", prev_edge[4], "from")
            print_subgrid([edges_with_direction_and_inside[j]
                          for j in indices_exp])
            shrink = (abs(center[4])+1)*abs(prev_edge[4])

            delete_candidate.append(indices[2])
            delete_candidate.append(indices[1])

            delete_candidate.append(indices[3])
            delete_candidate.append(indices[4])

            prev_prev[4] += center[4]
            prev_prev[4] += next_next[4]
            prev_prev[1] = next_position(
                prev_prev[0], prev_prev[2], abs(prev_prev[4]))
            modified_edge = prev_prev
            modified_idx = indices[0]
        elif abs(prev_edge[4]) < abs(next_edge[4]):
            # prev_edge will vanish.
            # check if we create a there and back edge
            if prev_prev[2] != center[2]:

                continue

            print("Cutting", prev_edge, center, next_edge, prev_prev,
                  "change pos", next_edge[0], "by", prev_edge[4], "from")
            print_subgrid([edges_with_direction_and_inside[j]
                          for j in indices_exp])
            shrink = (abs(center[4])+1)*abs(prev_edge[4])
            prev_prev[4] += center[4]
            prev_prev[1] = next_position(
                prev_prev[0], prev_prev[2], abs(prev_prev[4]))

            next_edge[4] += prev_edge[4]
            next_edge[0] = next_position(
                next_edge[1], next_edge[2], -abs(next_edge[4]))
            delete_candidate.append(indices[2])
            delete_candidate.append(indices[1])
            modified_edge = prev_prev
            modified_idx = indices[0]
        else:
            if next_next[2] != center[2]:
                continue

            # delete next edge
            print("Cutting", prev_edge, center, next_edge, next_next,
                  "change pos", prev_edge[0], "by", next_edge[4], "from")
            print_subgrid([edges_with_direction_and_inside[j]
                          for j in indices_exp])

            next_next[4] += center[4]
            next_next[0] = next_position(
                next_next[1], next_next[2], -abs(next_next[4]))

            prev_edge[4] += next_edge[4]
            prev_edge[1] = next_position(
                prev_edge[0], prev_edge[2], abs(prev_edge[4]))
            delete_candidate.append(indices[2])
            delete_candidate.append(indices[3])

            shrink = (abs(center[4])+1)*abs(next_edge[4])
            modified_edge = next_next
            modified_idx = indices[4]

        is_good = True
        for j in range(len(edges_with_direction_and_inside)):
            if j not in indices and abs(modified_idx-j) > 2:
                if edges_cross(modified_edge, edges_with_direction_and_inside[j]):
                    print("WARN, crossing ", modified_idx, j,
                          modified_edge, edges_with_direction_and_inside[j])
                    is_good = False
        if is_good:
            for i in indices_exp:
                modified[i] = True
            for idx in delete_candidate:
                delete.append(idx)
            print("after: Shrink by ", shrink)
            print_subgrid([edges_with_direction_and_inside[j]
                           for j in indices_exp if j not in delete_candidate])
            sum_cut += shrink

        else:
            for idx, val in zip(indices, backup):
                edges_with_direction_and_inside[idx] = val

    if len(delete) == 0:
        print("No cut found", reduce_step)
        break
    delete.sort(reverse=True)
    print("cutting", len(delete), "now the grid looks like")
    for val in delete:
        edges_with_direction_and_inside.pop(val)
    print_subgrid(edges_with_direction_and_inside)

    # print("Edges:", edges_with_direction_and_inside)

    # input("Press Enter to continue...")
print("Edges:", edges_with_direction_and_inside)
sum_cut += (abs(edges_with_direction_and_inside[0][4])+1)*(
    abs(edges_with_direction_and_inside[1][4])+1)
print(sum_cut)

# for reduced grid: 43990
