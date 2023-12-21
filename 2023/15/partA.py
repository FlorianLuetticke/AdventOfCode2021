from collections import defaultdict
import os


source_file_dir = os.path.dirname(os.path.abspath(__file__))


with open(source_file_dir+"/"+"final.txt", "r", encoding="utf8") as f:

    grid = []
    for line in f:
        line_new = line.replace("\n", "")
        grid = line_new.split(",")
        sum_val = 0
        for val in grid:

            value = 0
            for s in val:
                value += ord(s)
                value *= 17
                value = value % 256
            print("Input:", val, "Output:", value)
            sum_val += value

        print("Sum:", sum_val)
