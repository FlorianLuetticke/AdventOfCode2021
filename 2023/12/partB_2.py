from collections import defaultdict
import os


source_file_dir = os.path.dirname(os.path.abspath(__file__))

filename = "final.txt"

possible_small = []
mult = 5

# Debug functions to solve at  mult=2, mult=3.


def isPossible(pattern, cur_block_size):
    res = list(filter(None, "".join(pattern).split(".")))
    if len(res) != len(cur_block_size):
        return 0
    if all([len(res[j]) == cur_block_size[j] for j in range(len(res))]):
        # print("found pattern", "".join(pattern),
        #      "for", i, "with", cur_block_size)
        return 1
    return 0


def isPossibleAtStart(pattern, cur_block_size, exp_true, exp_false, sum_true, sum_false, sum_maybe):
    res = list(filter(None, "".join(pattern).split(".")))
    # print("isPossibleAtStart", "".join(pattern), cur_block_size, res)

    # number of elements in res not containing ?

    # print("isPossibleAtStart", "".join(pattern), cur_block_size, res, expTrue,
    #       expFalse, sum_true, sum_false, sum_maybe)

    if exp_true > sum_true+sum_maybe or exp_false > sum_false+sum_maybe:
        return False
    if sum([1 for s in res if "?" not in s]) > len(cur_block_size):
        return False
    for j in range(min(len(res), len(cur_block_size))):
        if "?" in res[j]:
            len_before = res[j].index("?")
            # print("len_before", len_before, cur_block_size[j], len(res[j]))
            return cur_block_size[j] >= len_before and (cur_block_size[j] <= len(res[j]) or "#" not in res[j])
        elif len(res[j]) != cur_block_size[j]:
            return False
    return True


def countPossibilitiesStart(pattern: list, block_sizes):
    expTrue = sum(block_sizes)
    expFalse = len(pattern)-expTrue
    sum_true = pattern.count("#")
    sum_false = pattern.count(".")
    sum_maybe = pattern.count("?")
    print("countPossibilitiesStart", "".join(pattern), block_sizes, expTrue,
          expFalse, sum_true, sum_false, sum_maybe)
    return countPossibilities(pattern, block_sizes, expTrue,
                              expFalse, sum_true, sum_false, sum_maybe)


def countPossibilities(pattern: list, block_sizes, exp_true, exp_false, sum_true, sum_false, sum_maybe):

    if sum_maybe == 0:
        ret = isPossible(pattern, block_sizes)
        # print("found possible pattern", "".join(pattern), ret)
        return ret
    # get index of first missing in list
    missingIndex = pattern.index("?")

    ret = 0
    pattern[missingIndex] = "."

    if isPossibleAtStart(pattern, block_sizes, exp_true, exp_false, sum_true, sum_false+1, sum_maybe-1):
        # print("pattern", "".join(pattern), "might be possible", block_sizes)
        ret += countPossibilities(pattern, block_sizes, exp_true,
                                  exp_false, sum_true, sum_false+1, sum_maybe-1)
    pattern[missingIndex] = "#"
    if isPossibleAtStart(pattern, block_sizes, exp_true, exp_false, sum_true+1, sum_false, sum_maybe-1):
        # print("pattern", "".join(pattern), "might be possible", block_sizes)
        ret += countPossibilities(pattern, block_sizes, exp_true,
                                  exp_false, sum_true+1, sum_false, sum_maybe-1)
    pattern[missingIndex] = "?"
    return ret


class Block:
    def __init__(self, size, pattern):
        self.size = size
        self.pattern = pattern
        self.possible_starts = [i for i in range(
            len(pattern)-size+1) if all([s == "?" or s == "#" for s in pattern[i:i+size]]) and (i == 0 or pattern[i-1] != "#") and (i+size == len(pattern) or pattern[i+size] != "#")]
        self.independent = False
        self.independent_starts = []
        self.dependent_starts = []
        self.possibilityMap = {}
        # print([pattern[i:i+size] for i in range(len(pattern)-size+1)])

    def __repr__(self):
        return f"Block({self.size},  {self.possible_starts})"

    def cut_from_previous(self, prev):
        self.possible_starts = [
            i for i in self.possible_starts if i > prev.possible_starts[0]+prev.size]
        return self

    def cut_from_nxt(self, nxt):
        # TODO: Start is only possible if there is no block of #  between. This is not checked yet
        self.possible_starts = [
            i for i in self.possible_starts if i+self.size < nxt.possible_starts[-1]]
        return self

    def cut_impossible(self, prev, nxt):
        new_possible_starts = []
        for start in self.possible_starts:
            prev_start = 0
            if prev is not None:
                prev_start = prev.possible_starts[-1] + prev.size
            nxt_start = len(self.pattern)
            if nxt is not None:
                nxt_start = nxt.possible_starts[0]
            if "#" not in self.pattern[prev_start:start] and "#" not in self.pattern[start+self.size:nxt_start]:
                new_possible_starts.append(start)
            # else:
            #     print("Invalid:", self, prev, nxt, start,
            #           self.pattern[prev_start:start], self.pattern[start+self.size:nxt_start])

        self.possible_starts = new_possible_starts

    def independent_from_previous(self, prev):
        return self.possible_starts[0] > prev.possible_starts[-1]+prev.size

    def independent_from_nxt(self, nxt):
        return self.possible_starts[-1]+self.size < nxt.possible_starts[0]

    def check_independence(self, prev, nxt):
        if (nxt is None):
            self.independent = self.independent_from_previous(prev)
        elif (prev is None):
            self.independent = self.independent_from_nxt(nxt)
        else:
            self.independent = self.independent_from_previous(
                prev) and self.independent_from_nxt(nxt)
        for i in range(len(self.possible_starts)):
            if (prev is None or self.possible_starts[i] > prev.possible_starts[-1]+prev.size) and (nxt is None or self.possible_starts[i]+self.size < nxt.possible_starts[0]):
                self.independent_starts.append(self.possible_starts[i])
            else:
                self.dependent_starts.append(self.possible_starts[i])


with open(source_file_dir+"/"+filename, "r", encoding="utf8") as f:
    patterns = []
    missingCount = []
    block_sizes = []

    for line in f:

        print(line)

        print(",".join([line.split()[1]]*mult))
        block_sizes.append([int(s)
                           for s in ",".join([line.split()[1]]*mult).split(",")])
        patterns.append([s for s in "?".join([line.split()[0]]*mult)])
        missingCount.append(patterns[-1].count("?"))

    print(patterns)
    print(block_sizes)
    print(missingCount)
    results = []

    debug_list = []
    for i in range(len(patterns)):
        blocks = [Block(s, patterns[i]) for s in block_sizes[i]]
        print("\n  ", i)
        for j in range(len(blocks)-1):
            blocks[j+1].cut_from_previous(blocks[j])

        for j in range(len(blocks)-1, 0, -1):
            blocks[j-1].cut_from_nxt(blocks[j])
        # print(i, block_sizes[i])
        # print(i, "".join(patterns[i]))
        blocks[0].cut_impossible(None, blocks[1])
        for j in range(len(blocks)-2):
            blocks[j+1].cut_impossible(blocks[j], blocks[j+2])
        blocks[-1].cut_impossible(blocks[-2], None)
        # backwards: (because forward cut have cut something which made previous invalid)
        for j in range(len(blocks)-2, 0, -1):
            blocks[j].cut_impossible(blocks[j-1], blocks[j+1])
        blocks[0].cut_impossible(None, blocks[1])

        print("".join(patterns[i]), block_sizes[i])
        print(blocks)
        # loop backwards over blocks. Count possiblities. Last block has one possiblity  for each start
        blocks[-1].possibilityMap = {
            start: 1 for start in blocks[-1].possible_starts}

        for j in range(len(blocks)-2, -1, -1):
            for start in blocks[j].possible_starts:
                sum_pos = 0
                for other_start in blocks[j+1].possible_starts:
                    if other_start > start+blocks[j].size:
                        if "#" not in blocks[j].pattern[start+blocks[j].size:other_start]:
                            sum_pos += blocks[j +
                                              1].possibilityMap[other_start]
                        # else:
                        #     print(
                        #         "Invalid case!:", blocks[j].pattern[start+blocks[j].size:other_start], start, other_start)

                blocks[j].possibilityMap[start] = sum_pos
            # print(j, blocks[j].possibilityMap)
        sum_possibilities = sum(blocks[0].possibilityMap.values())
        print([(j, blocks[j].possibilityMap)
              for j in range(len(blocks)-1, -1, -1)])
        print("Result", i, sum_possibilities)
        results.append(sum_possibilities)
        debug_list.append((i, patterns[i], block_sizes[i], sum_possibilities))

    print(results)
    print(sum(results))
