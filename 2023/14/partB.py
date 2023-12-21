import numpy as np
import os

source_file_dir = os.path.dirname(os.path.abspath(__file__))


with open(source_file_dir+"/"+"final.txt", "r", encoding="utf8") as f:

    replace = {".": 0, "#": 1, "O": 2}
    rep_inv = {val: key for key, val in replace.items()}
    grid = []
    for line in f:
        grid.append([replace[s] for s in line.strip()])
    print("\n".join(["".join(map(lambda x: rep_inv[x], l)) for l in
                     grid]))

    # move north
    ngrid = np.array(grid)
    results = []
    found_at = None
    for iterations in range(1000):
        for rot_cnt in range(4):
            for i in range(ngrid.shape[0]):
                for j in range(ngrid.shape[1]):

                    # find best place above:
                    if ngrid[i, j] == 2:
                        best_index = i
                        for subi in range(i-1, -1, -1):
                            if ngrid[subi, j] == 0:
                                best_index = subi
                            else:
                                break
                        ngrid[i, j] = 0
                        ngrid[best_index, j] = 2
            # print("After Rotation", rot_cnt)
            # print_arr = ngrid
            # if rot_cnt != 0:
            #     print_arr = np.rot90(print_arr, rot_cnt)
            # print("\n".join(["".join(map(lambda x: rep_inv[x], l))
            #                  for l in print_arr]))
            ngrid = np.rot90(ngrid, -1)
        # print("After iteration", iterations)
        # print("\n".join(["".join(map(lambda x: rep_inv[x], l))
        #                  for l in ngrid]))
        results.append(np.copy(ngrid))
        found = False
        for search_idx in range(len(results)-1):
            if np.array_equal(results[search_idx], results[-1]):
                found_at = search_idx
                break
        if found_at is not None:
            break

    print("last result at index:", found_at, "and", len(results)-1)
    sequenceLen = len(results)-1 - found_at
    # because idx 0 is after one iteration.
    targetCount = 1000000000 - found_at - 1

    in_sequence = targetCount % sequenceLen
    print(sequenceLen, "%", targetCount, "=", in_sequence)
    ngrid = results[found_at+in_sequence]

    print("\n".join(["".join(map(lambda x: rep_inv[x], l))
                     for l in ngrid]))

    load = 0
    for i in range(ngrid.shape[0]):
        for j in range(ngrid.shape[1]):
            if ngrid[i, j] == 2:
                load += ngrid.shape[0]-i
    print(load)
