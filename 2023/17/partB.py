from collections import defaultdict
import os
import heapq
from copy import deepcopy

source_file_dir = os.path.dirname(os.path.abspath(__file__))

grid = []
with open(source_file_dir+"/"+"final.txt", "r", encoding="utf8") as f:
    for line in f:
        grid.append([int(s) for s in line.strip()])
    print(grid)
    print("\n".join(["".join(map(str, l)) for l in grid]))


directions = ["up", "right", "down", "left"]


class PathState:
    def __init__(self, pos, direction, straight):
        self.pos = pos
        self.direction = direction
        self.straight = straight

    def next_steps(self):
        ret = []
        if self.straight < 10:
            ret.append(PathState(self.next_pos(self.direction),
                       self.direction, self.straight+1))
        if self.straight >= 4:
            directions.index(self.direction)
            for new_direction in directions[(directions.index(self.direction)+1) % 4], directions[(directions.index(self.direction)-1) % 4]:
                ret.append(PathState(self.next_pos(
                    new_direction), new_direction, 1))
        return ret

    def next_pos(self, new_direction):
        if new_direction == "left":
            return (self.pos[0], self.pos[1]-1)
        elif new_direction == "right":
            return (self.pos[0], self.pos[1]+1)
        elif new_direction == "up":
            return (self.pos[0]-1, self.pos[1])
        elif new_direction == "down":
            return (self.pos[0]+1, self.pos[1])

    def __repr__(self):
        return f"PS({self.pos}, {self.direction}, {self.straight})"

    def __lt__(self, other):
        return self.pos < other.pos


# left, right, up, down
# create a graph with all the edges


# Create a priority queue
priority_queue = []

shortestPath = [[defaultdict(lambda: 100000000000000) for _ in range(len(grid[0]))]
                for _ in range(len(grid))]

shortest_origin = [[defaultdict(lambda: 100000000000000) for _ in range(len(grid[0]))]
                   for _ in range(len(grid))]

# Add elements to the queue
heapq.heappush(priority_queue, (-grid[0][0], PathState((0, 0), 'down', 0)))
heapq.heappush(priority_queue, (-grid[0][0], PathState((0, 0), 'right', 0)))


while len(priority_queue) > 0:
    cur = heapq.heappop(priority_queue)
    # improvement: while straight smaller than 4, only add straight
    print(cur)
    cost = grid[cur[1].pos[0]][cur[1].pos[1]] + cur[0]
    if cur[1].pos == (len(grid)-1, len(grid[0])-1):
        print("result", cost)
        break
    if cur[0] > shortestPath[cur[1].pos[0]][cur[1].pos[1]][(cur[1].direction, cur[1].straight)]:
        print("ignored because cheaper was found already for", cur[1])
        continue

    for next_step in cur[1].next_steps():
        # print("possible next", next_step)
        if next_step.pos[0] < 0 or next_step.pos[0] >= len(grid) or next_step.pos[1] < 0 or next_step.pos[1] >= len(grid[0]):
            continue
        if cost < shortestPath[next_step.pos[0]][next_step.pos[1]][(next_step.direction, next_step.straight)]:
            shortestPath[next_step.pos[0]][next_step.pos[1]
                                           ][(next_step.direction, next_step.straight)] = cost
            shortest_origin[next_step.pos[0]][next_step.pos[1]
                                              ][(next_step.direction, next_step.straight)] = cur[1]
            heapq.heappush(priority_queue, (cost, next_step))
        else:
            print("skipped due to cost: ", cost, "vs",
                  shortestPath[next_step.pos[0]][next_step.pos[1]][(next_step.direction, next_step.straight)], next_step)

    # print("done")
    # print(priority_queue)

# print path
grid_cp = deepcopy(grid)
direction_short = {"up": "^", "right": ">", "down": "v", "left": "<"}
cur = cur[1]

path_pos = []
while True:
    path_pos.append(cur.pos)
    if cur.pos == (0, 0):
        break
    pos = cur.pos
    direction_string = direction_short[cur.direction]
    grid_cp[pos[0]][pos[1]] = direction_string
    cur = shortest_origin[pos[0]][pos[1]][(cur.direction, cur.straight)]
print("\n".join(["".join(map(str, l)) for l in grid_cp]))
print(path_pos)
print([grid[p[0]][p[1]] for p in path_pos])
print(sum(grid[p[0]][p[1]] for p in path_pos)-grid[0][0])
