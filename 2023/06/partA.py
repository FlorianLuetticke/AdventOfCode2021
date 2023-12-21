from collections import defaultdict
import os


source_file_dir = os.path.dirname(os.path.abspath(__file__))


with open(source_file_dir+"/"+"final.txt", "r", encoding="utf8") as f:

    lines = f.readlines()
    print(lines)
    times = [int(s) for s in lines[0].split(":")[1].split()]
    dist = [int(s) for s in lines[1].split(":")[1].split()]
    tot = 1
    for d, t in zip(dist, times):
        print(d, t)
        result = sum(map(lambda x: d < (x*(t-x)), range(t)))
        tot *= result
        print(result)
    print(tot)
