from collections import defaultdict
import os


source_file_dir = os.path.dirname(os.path.abspath(__file__))

grid = []
with open(source_file_dir+"/"+"final.txt", "r", encoding="utf8") as f:

    for line in f:
        print(line.strip())

        grid.append([x for x in line.strip()])


can_reach_even = [[False for x in range(len(grid[0]))]
                  for y in range(len(grid))]
can_reach_odd = [[False for x in range(len(grid[0]))]
                 for y in range(len(grid))]

start_x = [1 if any([x == "S" for x in row]) else 0 for row in grid].index(1)
start_y = grid[start_x].index("S")

queue = [((start_x, start_y), 0)]


def next_pos(x, y, grid):
    ret = []
    if x > 0 and grid[x-1][y] != "#":
        ret.append((x-1, y))
    if x < len(grid)-1 and grid[x+1][y] != "#":
        ret.append((x+1, y))
    if y > 0 and grid[x][y-1] != "#":
        ret.append((x, y-1))
    if y < len(grid[0])-1 and grid[x][y+1] != "#":
        ret.append((x, y+1))
    return ret


max_steps = 64
valid = []
while len(queue) > 0:
    pos, steps = queue.pop(0)
    if steps == max_steps:
        if steps % 2 == 0:
            can_reach_even[pos[0]][pos[1]] = True
        else:
            can_reach_odd[pos[0]][pos[1]] = True
        valid.append(pos)
        continue
    if can_reach_even[pos[0]][pos[1]] and steps % 2 == 0:
        continue
    if can_reach_odd[pos[0]][pos[1]] and steps % 2 == 1:
        continue
    if steps % 2 == 0:
        can_reach_even[pos[0]][pos[1]] = True
    else:
        can_reach_odd[pos[0]][pos[1]] = True
    for n in next_pos(pos[0], pos[1], grid):
        queue.append((n, steps+1))

print(valid)
print("Grid size", len(grid), len(grid[0]))

print("\n".join(["".join(map(lambda x: str(int(x)), l))
      for l in can_reach_even]))
# print("\n".join(["".join(map(lambda x: str(int(x)), l))
#       for l in can_reach_odd]))

for idx, row in enumerate(can_reach_even):
    for idy, val in enumerate(row):
        if val:
            if (grid[idx][idy] == "#"):
                print("ERROR at", idx, idy)
            grid[idx][idy] = "O"
print("\n".join(["".join(map(str, l)) for l in grid]))
print("even", sum(sum(l) for l in can_reach_even))

# print("odd", sum(sum(l) for l in can_reach_odd))
