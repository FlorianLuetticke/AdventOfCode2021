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


def next_pos(pos, direction, length):
    if direction == "L":
        return (pos[0], pos[1]-length)
    elif direction == "R":
        return (pos[0], pos[1]+length)
    elif direction == "U":
        return (pos[0]-length, pos[1])
    elif direction == "D":
        return (pos[0]+length, pos[1])


cur_pos = (0, 0)
pos_list = [cur_pos]
for command in commands:
    cur_pos = next_pos(cur_pos, command[0], command[1])
    pos_list.append(cur_pos)
print(pos_list)

# get min/max x/y
min_x = min([pos[0] for pos in pos_list])
max_x = max([pos[0] for pos in pos_list])
min_y = min([pos[1] for pos in pos_list])
max_y = max([pos[1] for pos in pos_list])
print(min_x, max_x, min_y, max_y)
grid = [["." for _ in range(max_y-min_y+1)] for _ in range(max_x-min_x+1)]
for i in range(len(pos_list)-1):
    start = pos_list[i]
    end = pos_list[i+1]
    for i in range(min(start[0], end[0]), max(start[0], end[0])+1):
        for j in range(min(start[1], end[1]), max(start[1], end[1])+1):
            grid[i-min_x][j-min_y] = "#"

print("\n".join(["".join(map(str, l)) for l in grid]))

queue = []
for i in range(len(grid)):

    if grid[i][0] == ".":
        queue.append((i, 0))
    if grid[i][-1] == ".":
        queue.append((i, len(grid[i])-1))
for j in range(len(grid[0])):
    if grid[0][j] == ".":
        queue.append((0, j))
    if grid[-1][j] == ".":
        queue.append((len(grid)-1, j))


def get_adjacent(pos):
    ret = []
    for i in range(-1, 2):
        for j in range(-1, 2):
            if i == 0 and j == 0:
                continue
            if i != 0 and j != 0:
                continue
            if pos[0]+i >= 0 and pos[0]+i < len(grid) and pos[1]+j >= 0 and pos[1]+j < len(grid[0]):
                ret.append((pos[0]+i, pos[1]+j))
    return ret


while len(queue) > 0:

    cur = queue.pop(0)
    if grid[cur[0]][cur[1]] != ".":
        continue
    grid[cur[0]][cur[1]] = "O"

    for adj in get_adjacent(cur):
        if grid[adj[0]][adj[1]] == ".":
            queue.append(adj)


print("\n".join(["".join(map(str, l)) for l in grid]))


count = sum([sum([1 if grid[i][j] == "O" else 0 for j in range(len(grid[0]))])
            for i in range(len(grid))])
print(count, len(grid)*len(grid[0]))
print(len(grid)*len(grid[0])-count)
