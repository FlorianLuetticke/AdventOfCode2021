from collections import defaultdict
import os
import heapq
from copy import deepcopy
import numpy
source_file_dir = os.path.dirname(os.path.abspath(__file__))

commands = []
directions_map = {"0": "R", "1": "D", "2": "L", "3": "U"}

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


edges_with_direction_and_inside = []
cur_pos = (0, 0)

for command in commands:
    next_pos = next_position(cur_pos, command[0], command[1])
    edges_with_direction_and_inside.append(
        [cur_pos, next_pos, command[0], "?", command[1] if command[0] in ["D", "R"] else -command[1]])
    cur_pos = next_pos


def create_reduced_grid(edges):

    x_set = {edge[0][0] for edge in edges}
    y_set = {edge[0][1] for edge in edges}

    x_l = list(x_set)
    y_l = list(y_set)
    x_l.sort()
    y_l.sort()
    x_dict = {val: 2*idx for idx, val in enumerate(x_l)}
    y_dict = {val: 2*idx for idx, val in enumerate(y_l)}
    reduced = []
    print(x_dict)
    print(y_dict)
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


edges_with_direction_and_inside = create_reduced_grid(
    edges_with_direction_and_inside)


def get_subgrid_int(edges):

    min_x_ = min([edge[0][0] for edge in edges] + [edge[1][0]
                 for edge in edges])
    max_x_ = max([edge[0][0] for edge in edges] + [edge[1][0]
                 for edge in edges])
    min_y_ = min([edge[0][1] for edge in edges] + [edge[1][1]
                 for edge in edges])
    max_y_ = max([edge[0][1] for edge in edges] + [edge[1][1]
                 for edge in edges])
    grid_ = [[0 for _ in range(max_y_-min_y_+1)]
             for _ in range(max_x_-min_x_+1)]

    for edge in edges:
        start_ = edge[0]
        end_ = edge[1]
        for i in range(min(start_[0], end_[0]), max(start_[0], end_[0])+1):
            for j in range(min(start_[1], end_[1]), max(start_[1], end_[1])+1):

                grid_[i-min_x_][j-min_y_] = 1
    return grid_


def count_simple(edges):

    grid = get_subgrid_int(edges)

    def get_adjacent(pos):
        ret = []
        for i in range(-1, 2):
            for j in range(-1, 2):
                if i == 0 and j == 0:
                    continue
                if pos[0]+i >= 0 and pos[0]+i < len(grid) and pos[1]+j >= 0 and pos[1]+j < len(grid[0]):
                    ret.append((pos[0]+i, pos[1]+j))
        return ret

    queue = []
    for i in range(len(grid)):

        if grid[i][0] == 0:
            queue.append((i, 0))
        if grid[i][-1] == 0:
            queue.append((i, len(grid[i])-1))
    for j in range(len(grid[0])):
        if grid[0][j] == 0:
            queue.append((0, j))
        if grid[-1][j] == 0:
            queue.append((len(grid)-1, j))
    while len(queue) > 0:

        cur = queue.pop(0)
        if grid[cur[0]][cur[1]] != 0:
            continue
        grid[cur[0]][cur[1]] = 2

        for adj in get_adjacent(cur):
            if grid[adj[0]][adj[1]] == 0:
                queue.append(adj)
    count = sum([sum([1 if grid[i][j] == 2 else 0 for j in range(len(grid[0]))])
                 for i in range(len(grid))])
    return len(grid)*len(grid[0]) - count


print(count_simple(edges_with_direction_and_inside))
