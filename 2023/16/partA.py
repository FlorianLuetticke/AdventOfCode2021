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


with open(source_file_dir+"/"+"final.txt", "r", encoding="utf8") as f:

    grid = []
    energized = []
    for line in f:
        grid.append([s for s in line.strip()])
        energized.append([False for _ in range(len(grid[-1]))])
    print(grid)
    print("\n".join(["".join(l) for l in grid]))
    pos_arr = [(0, -1, "right")]
    visited_by_dir = [[[]
                       for _ in range(len(grid[0]))] for _ in range(len(grid))]

    while len(pos_arr) > 0:
        cur = pos_arr.pop(0)
        print(cur)
        new_pos = next_pos(cur[:2], cur[2])
        if new_pos[0] < 0 or new_pos[0] >= len(grid) or new_pos[1] < 0 or new_pos[1] >= len(grid[0]):
            continue

        energized[new_pos[0]][new_pos[1]] = True
        grid_value = grid[new_pos[0]][new_pos[1]]
        print("new pos", new_pos[0], new_pos[1], grid[new_pos[0]][new_pos[1]])

        for direction in next_pix[grid_value][cur[2]]:
            if direction not in visited_by_dir[new_pos[0]][new_pos[1]]:
                visited_by_dir[new_pos[0]][new_pos[1]].append(direction)
                pos_arr.append((new_pos[0], new_pos[1], direction))
            else:
                print("already visited", new_pos[0], new_pos[1], direction)
        # print("\n".join(["".join(["#" if s else "." for s in l])
        #                  for l in energized]))

    print(sum([sum([1 if s else 0 for s in l]) for l in energized]))
