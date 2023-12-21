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


def check(grid, start):
    pos_arr = [start]
    energized = [[False for _ in range(len(grid[0]))]
                 for _ in range(len(grid))]
    visited_by_dir = [[[]
                       for _ in range(len(grid[0]))] for _ in range(len(grid))]
    while len(pos_arr) > 0:
        cur = pos_arr.pop(0)
        # print(cur)
        new_pos = next_pos(cur[:2], cur[2])
        if new_pos[0] < 0 or new_pos[0] >= len(grid) or new_pos[1] < 0 or new_pos[1] >= len(grid[0]):
            continue

        energized[new_pos[0]][new_pos[1]] = True
        grid_value = grid[new_pos[0]][new_pos[1]]

        for direction in next_pix[grid_value][cur[2]]:
            if direction not in visited_by_dir[new_pos[0]][new_pos[1]]:
                visited_by_dir[new_pos[0]][new_pos[1]].append(direction)
                pos_arr.append((new_pos[0], new_pos[1], direction))

    return sum(sum(1 if s else 0 for s in l) for l in energized)


with open(source_file_dir+"/"+"final.txt", "r", encoding="utf8") as f:

    grid = []
    for line in f:
        grid.append([s for s in line.strip()])
    print(len(grid), len(grid[0]))

    print("\n".join(["".join(l) for l in grid]))
    max_result = 0
    for i in range(len(grid)):

        v1 = check(grid, (i, -1, "right"))
        v2 = check(grid, (i, len(grid[0]), "left"))
        v3 = check(grid, (-1, i, "down"))
        v4 = check(grid, (len(grid), i, "up"))
        print(i, v1, v2, v3, v4)
        max_result = max(max_result, v1, v2, v3, v4)
    print("result", max_result)
