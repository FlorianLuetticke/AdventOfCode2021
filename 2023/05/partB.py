from collections import defaultdict
import os


source_file_dir = os.path.dirname(os.path.abspath(__file__))


class SeedRange:
    def __init__(self):
        self.subRanges = []

    def __repr__(self):
        return ", ".join(map(lambda x: str(x[0])+"-"+str(x[1]), self.subRanges))

    def addRange(self, start, end):
        self.subRanges.append([start, end])

    def combineRanges(self):
        self.subRanges.sort(key=lambda x: x[0])
        # new_sub_ranges = [self.subRanges[0]]

        # for i in range(1, len(self.subRanges)):
        #     # check if both ranges overlap
        #     if self.subRanges[i][0] <= new_sub_ranges[-1][1]:
        #         new_sub_ranges[-1][1] = max(
        #             new_sub_ranges[-1][1], self.subRanges[i][1])
        #     else:
        #         new_sub_ranges.append(self.subRanges[i])

        # self.subRanges = new_sub_ranges


class Rule:
    def __init__(self, line):
        self.dest_start = int(line.split()[0])
        self.source_start = int(line.split()[1])
        self.rule_length = int(line.split()[2])

    def __repr__(self):

        return str(self.source_start) + " -> " + str(self.dest_start)+" for  " + str(self.rule_length)

    def apply_range(self, input_range: SeedRange) -> (SeedRange, SeedRange):

        mapped_range = SeedRange()
        remaining_input_range = SeedRange()

        for sub in input_range.subRanges:

            # check if rule does not apply
            if self.source_start > sub[1] or self.source_start+self.rule_length-1 < sub[0]:
                remaining_input_range.addRange(sub[0], sub[1])
                continue
            if self.source_start <= sub[0] and self.source_start+self.rule_length-1 >= sub[1]:
                # print("rule applies fully", self.source_start,
                #       sub[0], self.source_start+self.rule_length-1, sub[1])
                mapped_range.addRange(
                    self.dest_start + sub[0] - self.source_start, self.dest_start + sub[1] - self.source_start)

                # if self.source_start+self.rule_length-1 > sub[1]:
                #     remaining_input_range.addRange(
                #         self.source_start+self.rule_length, sub[1])
                # if self.source_start < sub[0]:
                #     remaining_input_range.addRange(
                #         sub[0], self.source_start-1)
                continue

            if self.source_start <= sub[0]:
                # print("rule applies before", self.source_start,
                #       sub[0], self.source_start+self.rule_length-1, sub[1])
                mapped_range.addRange(
                    self.dest_start + sub[0] - self.source_start, self.dest_start + self.rule_length-1)
                remaining_input_range.addRange(
                    self.source_start + self.rule_length, sub[1])
                continue
            else:
                # print("rule applies after", self.source_start,
                #       sub[0], self.source_start+self.rule_length-1, sub[1])
                remaining_input_range.addRange(
                    sub[0], self.source_start-1)

                if self.source_start+self.rule_length-1 <= sub[1]:
                    remaining_input_range.addRange(
                        self.source_start+self.rule_length, sub[1])
                    mapped_range.addRange(
                        self.dest_start, self.dest_start + self.rule_length-1)
                else:
                    mapped_range.addRange(
                        self.dest_start, self.dest_start + sub[1] - self.source_start)

                continue
        mapped_range.subRanges.sort(key=lambda x: x[0])
        remaining_input_range.subRanges.sort(key=lambda x: x[0])
        return mapped_range, remaining_input_range


if __name__ == "__main__":
    with open(source_file_dir+"/"+"final.txt", "r", encoding="utf8") as f:

        points = 0
        seeds = SeedRange()
        rule_categories = defaultdict(list)
        next_category = {}
        current_category = None
        for line in f:

            if "seeds:" in line:
                seedsNr = [int(s) for s in line.split(":")[1].split()]
                for i in range(len(seedsNr)//2):
                    seeds.addRange(seedsNr[2*i], seedsNr[2*i]+seedsNr[2*i+1]-1)

                continue
            if "map:" in line:
                tokens = line.split()[0]
                current_category = tokens.split("-")[0]
                next_category[current_category] = tokens.split("-")[2]
                continue
            if len(line.strip()) == 0:
                continue
            rule_categories[current_category].append(Rule(line))
        print(seeds)
        print(rule_categories)

        cat = "seed"
        print("seed is", seeds)
        while cat != "location":
            print("for", cat)
            after_rule = SeedRange()
            for rule in rule_categories[cat]:
                mapped_seeds, seeds = rule.apply_range(seeds)
                print("mapped ", cat, mapped_seeds, "remaining", cat, seeds)
                after_rule.subRanges += mapped_seeds.subRanges
            print("missing rules for", seeds)
            after_rule.subRanges += seeds.subRanges
            print("uncompressed result ", after_rule)
            after_rule.combineRanges()
            print("compressed result ", after_rule)
            seeds = after_rule
            cat = next_category[cat]
            print("for", cat,  seeds)
        print(seeds.subRanges[0][0])
