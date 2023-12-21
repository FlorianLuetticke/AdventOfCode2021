from collections import defaultdict
import os
import numpy as np

source_file_dir = os.path.dirname(os.path.abspath(__file__))


def hasMirrorAxis(pict):
    ret = []
    mult = np.array([2**i for i in range(pict.shape[0])])
    print(mult)
    print(np.sum(pict*mult[:, None], axis=0))
    flat = np.sum(pict*mult[:, None], axis=0)
    for i in range(len(flat)-1):
        good = True
        for t1, t2 in zip(range(i+1, len(flat)), range(i, -1, -1)):
            if flat[t1] != flat[t2]:
                good = False
                break
        if good:
            print("found at", i+1, i+2)
            ret.append((i+1, i+2))
    return ret


def hasMirrorAxisWithSmudge(pict):
    ret = []
    mult = np.array([4**i for i in range(pict.shape[0])])
    # print(mult)
    # print(np.sum(pict*mult[:, None], axis=0))
    flat = np.sum(pict*mult[:, None], axis=0)
    for i in range(len(flat)-1):
        difference = []
        for t1, t2 in zip(range(i+1, len(flat)), range(i, -1, -1)):
            difference.append(abs(flat[t1]-flat[t2]))

        # print(i, difference)
        smudge_count = 0
        clear_count = 0
        for d in difference:
            if d in mult:
                smudge_count += 1
            if d == 0:
                clear_count += 1

        if smudge_count == 1 and clear_count+1 == len(difference):
            # print("found at", i+1, i+2)
            ret.append((i+1, i+2))

    return ret


with open(source_file_dir+"/"+"final.txt", "r", encoding="utf8") as f:
    picture = []
    pictureList = []
    for line in f:
        if len(line.strip()) == 0:
            # print(picture)
            pictureList.append(np.array(picture, dtype=int))
            picture = []
        else:
            picture.append([s == "#" for s in line.strip()])
    # print(pictureList)

    score = 0
    for pic in pictureList:

        axis = hasMirrorAxisWithSmudge(pic)
        # print(axis)
        if (len(axis) > 1):
            print("found more than one axis", axis)
        for a in axis:
            score += 1*a[0]
        picT = pic.transpose()

        axisT = hasMirrorAxisWithSmudge(picT)
        if (len(axisT) > 1):
            print("found more than one axisT", axisT)
            print(picT)
            for a in axisT:
                print(picT[:, :a[0]].transpose())
                print(picT[:, a[0]:].transpose())
        for a in axisT:
            score += 100*a[0]
        # print(axisT)
    print(score)
