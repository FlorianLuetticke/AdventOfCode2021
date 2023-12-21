from collections import defaultdict
import os
import numpy as np

source_file_dir = os.path.dirname(os.path.abspath(__file__))

multiple = 2
grid_factor = 2*multiple+1
grid = []
for i in range(grid_factor):
    with open(source_file_dir+"/"+"final.txt", "r", encoding="utf8") as f:

        for line in f:
            line = line.strip()
            if i != multiple:
                line = line.replace("S", ".")
                line *= grid_factor
            if i == multiple:
                line_without = line.replace("S", ".")
                line = line_without*multiple + line + line_without*multiple

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


max_steps = 131*multiple+65

while len(queue) > 0:
    pos, steps = queue.pop(0)
    if steps == max_steps:
        if steps % 2 == 0:
            can_reach_even[pos[0]][pos[1]] = True
        else:
            can_reach_odd[pos[0]][pos[1]] = True

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

print("Grid size", len(grid), len(grid[0]))

# print("\n".join(["".join(map(lambda x: str(int(x)), l))
#       for l in can_reach_even]))
# print("\n".join(["".join(map(lambda x: str(int(x)), l))
#       for l in can_reach_odd]))

for idx, row in enumerate(can_reach_even):
    for idy, val in enumerate(row):
        if val:
            if (grid[idx][idy] == "#"):
                print("ERROR at", idx, idy)
            grid[idx][idy] = "O"
# print("\n".join(["".join(map(str, l)) for l in grid]))
# print("even", sum(sum(l) for l in can_reach_even))


# split grid in 7x7 maps
# that works best with numpy
np_grid = np.array(grid)
np_even = np.array(can_reach_even)
np_odd = np.array(can_reach_odd)

# split numpy arrays in equal parts of size x,y:

x = len(grid)//grid_factor
y = len(grid[0])//grid_factor

grids2D = [np.split(g, grid_factor, axis=0)
           for g in np.split(np_grid, grid_factor, axis=1)]
even2D = [np.split(g, grid_factor, axis=0)
          for g in np.split(np_even, grid_factor, axis=1)]
odd2D = [np.split(g, grid_factor, axis=0)
         for g in np.split(np_odd, grid_factor, axis=1)]
# for i, g in enumerate(grids2D):
#     for j, g2 in enumerate(g):
#         print(i, j, g2.shape)
#         print(g2)

dict_even = defaultdict(list)
dict_odd = defaultdict(list)
for i in range(grid_factor):
    for j in range(grid_factor):
        print(i, j, np.sum(even2D[i][j]), np.sum(odd2D[i][j]))
        dict_even[np.sum(even2D[i][j])].append((i, j))
        dict_odd[np.sum(odd2D[i][j])].append((i, j))

print(np_grid.shape)

# calc each of the 9 solutions: Start at corner, center of border and center of map.
# or better:

# nr_of_steps = 26501365
nr_of_steps = 131*3+65
# nr of steps on last map:
# 65 on first map. 131 for each map after that.

nr_of_steps_last = (nr_of_steps - 65) % 131
print("steps on last:", nr_of_steps_last, "map nr:", (nr_of_steps-65)//131)
nr_of_steps_on_edge = (nr_of_steps-130) % 262
print("steps on last_edge:", nr_of_steps_on_edge)


# we have a diagonal square. The inner part consists of full grids.
# How many are there?
# The Border has small diagonals (s). large diagnonals (l) and edges (e)
# inner part is a or b
# r= 4 inner part is a
# 0 0 0 s e s 0 0 0
# 0 0 s l a l s 0 0
# 0 s l a b a l s 0
# s l a b a b a l s
# e a b a b a b a e
# s l a b a b a l s
# 0 s l a b a l s 0
# 0 0 s l a l s 0 0
# 0 0 0 s e s 0 0 0
# #s = r  *4 types
# #l = r-1 *4 types
# #e = 1 *4 types
# #a =  1/2 *r*(r+1) + 1/2 * (r-1)*r = r^2
# #b = 1/2*r*(r-1) + 1/2*(r-1)*(r-2) = (r-1)%2
# r is multiple

# we can calculate for r = 2, evaluate for target r!
# our graph would be 5x5
# 0 s e s 0
# s l a l s
# e a b a e
# s l a l s
# 0 s e s 0

nr_of_steps = 26501365
# nr of steps on last map:
# 65 on first map. 131 for each map after that.
multiple_calc = (nr_of_steps - 65) // 131
print("calc for", multiple_calc)
even2D = odd2D  # we gave an odd number of steps!!

edge_count = np.sum(even2D[0][multiple])+np.sum(even2D[-1][multiple]) + \
    np.sum(even2D[multiple][0])+np.sum(even2D[multiple][-1])
small_diagonals = multiple_calc * (np.sum(even2D[0][multiple-1])+np.sum(
    even2D[0][multiple+1])+np.sum(even2D[-1][multiple-1])+np.sum(even2D[-1][multiple+1]))

print("edges", edge_count, "= ", np.sum(even2D[0][multiple]), "+", np.sum(
    even2D[-1][multiple]), "+", np.sum(even2D[multiple][-1]), "+", np.sum(even2D[multiple][-1]))
print("small diagonals", small_diagonals, "=", multiple_calc, "* (", np.sum(even2D[0][multiple-1]), "+", np.sum(
    even2D[0][multiple+1]), "+", np.sum(even2D[-1][multiple-1]), "+", np.sum(even2D[-1][multiple+1]), ")")
long_diagonals = (multiple_calc-1) * (np.sum(even2D[1][multiple-1])+np.sum(
    even2D[1][multiple+1])+np.sum(even2D[-2][multiple-1])+np.sum(even2D[-2][multiple+1]))
print("long diagonals", long_diagonals, "=", multiple_calc-1, "* (", np.sum(even2D[1][multiple-1]), "+", np.sum(
    even2D[1][multiple+1]), "+", np.sum(even2D[-2][multiple-1]), "+", np.sum(even2D[-2][multiple+1]), ")")

count_a = (multiple_calc) * (multiple_calc)  # * np.sum(even2D[1][multiple])
# * np.sum(even2D[1][multiple])
count_b = (multiple_calc-1) * (multiple_calc-1)
print("counts", count_a, count_b)
inner = count_a * np.sum(even2D[1][multiple]) + \
    count_b * np.sum(even2D[2][multiple])
print("inner", count_a, "*",
      np.sum(even2D[1][multiple]), "+", count_b, "*", np.sum(even2D[2][multiple]))


print("total", edge_count+small_diagonals+long_diagonals+inner)


# test for multiple 3 -> even = 288587
# test for multiple 3 -> even = 175212
# test for multiple 2 -> even = 88945

# answer 584211381546803 too low (used even)
# answer 584211423220706 ok (we did odd number of steps!)
