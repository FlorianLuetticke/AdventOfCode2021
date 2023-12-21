from collections import defaultdict
import os


source_file_dir = os.path.dirname(os.path.abspath(__file__))


class Rule:
    def __init__(self, line):
        self.dest_start = int(line.split()[0])
        self.source_start = int(line.split()[1])
        self.rule_length = int(line.split()[2])

    def __repr__(self):

        return str(self.source_start) + " -> " + str(self.dest_start)+" for  " + str(self.rule_length)

    def applies(self, number):
        return number >= self.source_start and number <= self.source_start+self.rule_length-1

    def apply(self, number):
        return self.dest_start + number - self.source_start


with open(source_file_dir+"/"+"final.txt", "r", encoding="utf8") as f:

    points = 0
    seeds = []
    rule_categories = defaultdict(list)
    next_category = {}
    current_category = None
    for line in f:

        if "seeds:" in line:
            seeds = [int(s) for s in line.split(":")[1].split()]
            continue
        if "map:" in line:
            tokens = line.split()[0]
            current_category = tokens.split("-")[0]
            next_category[current_category] = tokens.split("-")[2]
            continue
        if len(line.strip()) == 0:
            continue
        rule_categories[current_category].append(Rule(line))

    print(rule_categories)
    min_seed = None
    for seed in seeds:
        cat = "seed"
        print("seed is", seed)
        while cat != "location":
            for rule in rule_categories[cat]:
                if rule.applies(seed):
                    seed = rule.apply(seed)
                    break
            cat = next_category[cat]
            print("for", cat, "seed is", seed)
        if min_seed == None or seed < min_seed:
            min_seed = seed
        print(seed)
    print(min_seed)
