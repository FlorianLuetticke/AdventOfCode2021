from collections import defaultdict
import os
import heapq
from copy import deepcopy

source_file_dir = os.path.dirname(os.path.abspath(__file__))

commands = []
with open(source_file_dir+"/"+"final.txt", "r", encoding="utf8") as f:
    for line in f:
        commands.append((line.split()[0], int(line.split()[1])))

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

print(pos_list[0], pos_list[-1])


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
    elif cur_inside == opposite[next_direction]:
        edge2[3] = cur_direction
    else:
        print("ERROR", edge1, edge2)
        exit(1)


# determine the inside direction

for count in range(2):
    for i in range(len(edges_with_direction_and_inside)-1):
        determine_dir(edges_with_direction_and_inside[i],
                      edges_with_direction_and_inside[i+1])

    determine_dir(
        edges_with_direction_and_inside[-1], edges_with_direction_and_inside[0])

print(edges_with_direction_and_inside)

print_map = {"U": "^", "D": "v", "L": "<", "R": ">"}


def print_grid(edges):
    grid_ = [["." for _ in range(max_y+1)]
             for _ in range(max_x+1)]
    for edge_ in edges:
        start_ = edge_[0]
        end_ = edge_[1]
        for i in range(min(start_[0], end_[0]), max(start_[0], end_[0])+1):
            for j in range(min(start_[1], end_[1]), max(start_[1], end_[1])+1):

                grid_[i][j] = print_map[edge_[3]]

    print("\n".join(["".join(map(str, l)) for l in grid_]))


def print_subgrid(edges):
    min_x_ = min([edge[0][0] for edge in edges] + [edge[1][0]
                 for edge in edges])
    max_x_ = max([edge[0][0] for edge in edges] + [edge[1][0]
                 for edge in edges])
    min_y_ = min([edge[0][1] for edge in edges] + [edge[1][1]
                 for edge in edges])
    max_y_ = max([edge[0][1] for edge in edges] + [edge[1][1]
                 for edge in edges])
    grid_ = [["." for _ in range(max_y_-min_y_+1)]
             for _ in range(max_x_-min_x_+1)]

    for edge in edges:
        start_ = edge[0]
        end_ = edge[1]
        for i in range(min(start_[0], end_[0]), max(start_[0], end_[0])+1):
            for j in range(min(start_[1], end_[1]), max(start_[1], end_[1])+1):

                grid_[i-min_x_][j-min_y_] = print_map[edge[3]]

    print("\n".join(["".join(map(str, l)) for l in grid_]))


def first_is_between(val1, val2, val3):
    return min(val3, val2) <= val1 <= max(val3, val2) and not (val1 == val2 and val1 == val3)


def edges_cross(edge1, edge2):
    if edge1[2] == edge2[2] or edge1[2] == opposite[edge2[2]]:
        return False
    if edge1[2] == "U" or edge1[2] == "D":
        return first_is_between(edge1[0][1], edge2[0][1], edge2[1][1]) and first_is_between(edge2[0][0], edge1[0][0], edge1[1][0])
    else:
        return first_is_between(edge1[0][0], edge2[0][0], edge2[1][0]) and first_is_between(edge2[0][1], edge1[0][1], edge1[1][1])


print("tests")
edge1 = [(5, 0), (5, 10), "R", "D", 1]
edge2 = [(0, 5), (10, 5), "D", "R", 1]
print(edges_cross(edge1, edge2))
print(edges_cross(edge2, edge1))

edge3 = [(5, 6), (5, 10), "R", "D", 1]
print(edges_cross(edge3, edge2))
print(edges_cross(edge2, edge3))
edge4 = [(5, 0), (5, 4), "R", "D", 1]
print(edges_cross(edge4, edge2))
print(edges_cross(edge2, edge4))

edge5 = [(6, 5), (10, 5), "D", "R", 1]
edge6 = [(0, 5), (4, 5), "D", "R", 1]

print(edges_cross(edge1, edge5))
print(edges_cross(edge5, edge1))
print(edges_cross(edge1, edge6))
print(edges_cross(edge6, edge1))

sum_cut = 0
print_grid(edges_with_direction_and_inside)
while len(edges_with_direction_and_inside) > 4:
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
        if any(modified[i] for i in indices):
            continue
        if prev_edge[2] != opposite[next_edge[2]]:
            print("No candidate:", prev_edge, center, next_edge)
            continue
        if prev_edge[2] == center[3] or next_edge[2] == opposite[center[3]]:
            print("No candidate because bad direction:",
                  prev_edge, center, next_edge)
            continue
        # cut: Either perv or next will vanish.
        if (abs(prev_edge[4]) == abs(next_edge[4])):
            if prev_prev[2] != center[2] or next_next[2] != center[2]:
                print("No candidate:", prev_edge, center,
                      next_edge, "because", prev_prev, "or", next_next)
                continue
            print("before - special")
            print_subgrid([edges_with_direction_and_inside[j]
                          for j in indices_exp])
            print("Cutting", prev_edge, center, next_edge, prev_prev)
            new_prev_prev = deepcopy(prev_prev)
            new_prev_prev[4] += center[4]
            new_prev_prev[4] += next_next[4]
            new_prev_prev[1] = next_position(
                new_prev_prev[0], new_prev_prev[2], abs(new_prev_prev[4]))
            for i in range(len(edges_with_direction_and_inside)):
                if i not in indices[:-1]:
                    if (edges_cross(new_prev_prev, edges_with_direction_and_inside[i])):
                        print("Crossing at ", new_prev_prev,
                              edges_with_direction_and_inside[i])
            prev_prev = new_prev_prev
            delete += indices[1:]
            sum_cut += (abs(center[4])+1)*abs(prev_edge[4])

        elif abs(prev_edge[4]) < abs(next_edge[4]):
            # prev_edge will vanish.
            # check if we create a there and back edge
            if prev_prev[2] != center[2]:
                print("No candidate:", prev_edge, center,
                      next_edge, "because", prev_prev)
                continue
            # cutFound = True
            print("before")
            print_subgrid([edges_with_direction_and_inside[j]
                          for j in indices_exp])
            print("Cutting", prev_edge, center, next_edge, prev_prev)
            print("change pos", next_edge[0], "by", prev_edge[4])
            sum_cut += (abs(center[4])+1)*abs(prev_edge[4])
            new_prev_prev = deepcopy(prev_prev)
            new_prev_prev[4] += center[4]
            new_prev_prev[1] = next_position(
                new_prev_prev[0], new_prev_prev[2], abs(new_prev_prev[4]))
            # check if it cuts:

            for i in range(len(edges_with_direction_and_inside)):
                if i not in indices[:-1]:
                    if (edges_cross(new_prev_prev, edges_with_direction_and_inside[i])):
                        print("Crossing at ", new_prev_prev,
                              edges_with_direction_and_inside[i])

            next_edge[4] += prev_edge[4]
            next_edge[0] = next_position(
                next_edge[1], next_edge[2], -abs(next_edge[4]))
            prev_prev = new_prev_prev
            delete.append(indices[2])
            delete.append(indices[1])

        else:
            if next_next[2] != center[2]:
                print("No candidate:", prev_edge, center,
                      next_edge, "because next", next_next)
                continue
            print("before")
            print_subgrid([edges_with_direction_and_inside[j]
                          for j in indices_exp])
            # delete next edge
            print("Cutting", prev_edge, center, next_edge, next_next)
            print("change pos", prev_edge[0], "by", next_edge[4])
            new_next_next = deepcopy(next_next)

            new_next_next[4] += center[4]
            new_next_next[0] = next_position(
                new_next_next[1], new_next_next[2], -abs(new_next_next[4]))

            for i in range(len(edges_with_direction_and_inside)):
                if i not in indices[:-1]:
                    if (edges_cross(new_next_next, edges_with_direction_and_inside[i])):
                        print("Crossing at ", new_next_next,
                              edges_with_direction_and_inside[i])

            prev_edge[4] += next_edge[4]
            prev_edge[1] = next_position(
                prev_edge[0], prev_edge[2], abs(prev_edge[4]))
            delete.append(indices[2])
            delete.append(indices[3])
            next_next = new_next_next
            sum_cut += (abs(center[4])+1)*abs(next_edge[4])
        print("after")
        print_subgrid([edges_with_direction_and_inside[j]
                       for j in indices_exp if j not in delete])
        for i in indices_exp:
            modified[i] = True

    if len(delete) == 0:
        print("No cut found")
        break
    delete.sort(reverse=True)
    print("cutting", len(delete))
    for val in delete:
        edges_with_direction_and_inside.pop(val)

    # check if edges still ok.

    for edge in edges_with_direction_and_inside:

        if edge[1] != next_position(edge[0], edge[2], abs(edge[4])):
            print("ERROR", edge)
            exit(1)
    for edge, next_edge in zip(edges_with_direction_and_inside, edges_with_direction_and_inside[1:]+edges_with_direction_and_inside[:1]):
        if edge[1] != next_edge[0]:
            print("ERROR", edge, next_edge)
            exit(1)

    print(edges_with_direction_and_inside)
    print_grid(edges_with_direction_and_inside)
    print(sum_cut)
    input("Press Enter to continue...")

sum_cut += (abs(edges_with_direction_and_inside[0][4])+1)*(
    abs(edges_with_direction_and_inside[1][4])+1)
print(sum_cut)
