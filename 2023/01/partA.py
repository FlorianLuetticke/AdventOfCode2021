import os


def result(filename):
    source_file_dir = os.path.dirname(os.path.abspath(__file__))
    n = 0
    with open(source_file_dir+"/"+filename, "r", encoding="utf8") as f:
        for line in f:
            # print(line)

            # extract all digits from the line
            v = [int(s) for s in line if s.isdigit()]

            n += v[0]*10 + v[-1]

    print(n)


result("test.txt")
result("final.txt")
