import os


source_file_dir = os.path.dirname(os.path.abspath(__file__))


with open(source_file_dir+"/"+"final.txt", "r", encoding="utf8") as f:

    grid = []
    for line in f:
        grid.append([s for s in line.strip()])
    print("\n".join(["".join(l) for l in grid]))

    # move north
    load = 0
    for i in range(len(grid)):
        for j in range(len(grid[0])):

            # find best place above:
            if grid[i][j] == "O":
                best_index = i
                for subi in range(i-1, -1, -1):
                    if grid[subi][j] == ".":
                        best_index = subi
                    else:
                        break
                grid[i][j] = "."
                load += len(grid)-best_index
                grid[best_index][j] = "O"
    print("result", load)

    print("\n".join(["".join(l) for l in grid]))
    print("result", load)
