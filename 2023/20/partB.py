from collections import defaultdict
import os
from copy import deepcopy
import math

source_file_dir = os.path.dirname(os.path.abspath(__file__))

rule_broadcast = 0
rule_inverter = 1
rule_flipflop = 2

rules_dict = {}

state_dict_inv = {}
state_dict_ff = {}
end_values = []
with open(source_file_dir+"/"+"final.txt", "r", encoding="utf8") as f:

    for line in f:
        print(line.strip())

        p = line.split(" -> ")

        name = p[0][1:]
        print(p[1].split(","))
        rules_dict[name] = (p[0][0], [x.strip() for x in p[1].split(",")])


for name, (rule, receivers) in rules_dict.items():
    if rule == "&":
        state_dict_inv[name] = {}
    if rule == "%":
        state_dict_ff[name] = "off"

for name, (rule, receivers) in rules_dict.items():
    for reciever in receivers:
        print(rule, reciever)
        if reciever in state_dict_inv:
            state_dict_inv[reciever][name] = "l"
    for reciever in receivers:
        if reciever not in rules_dict:
            print("end node", reciever)
            end_values.append(reciever)


print(rules_dict)

# create a grahviz digraph of rules_dict
#
with open(source_file_dir+"/"+"graph.dot", "w", encoding="utf8") as f:
    f.write("digraph G {\n")

    def escaped(s):
        if s == "%":
            return "ff_"
        if s == "&":
            return "con_"
        return s
    for node, info in rules_dict.items():
        for reciever in info[1]:
            if reciever in rules_dict:

                f.write(escaped(info[0])+node+" -> " +
                        escaped(rules_dict[reciever][0])+reciever+"\n")
            else:
                f.write(escaped(info[0])+node+" -> "+reciever+"\n")
    f.write("}")
print(state_dict_inv)
print(state_dict_ff)

low_pulse = 0
high_pulse = 0
interesting_nodes = [name for name,
                     (rule, receivers) in rules_dict.items() if rule == "&" and len(receivers) > 1]
print(interesting_nodes)

input("Press enter to continue")
interesting_emmissions = defaultdict(list)
sequence_of_states = []
for i in range(50000):
    queue = [("roadcaster", "l", None)]
    end_pulses = 0

    interesting_states = {}
    for node in interesting_nodes:
        interesting_states[node] = deepcopy(state_dict_inv[node])

    sequence_of_states.append(interesting_states)
    while len(queue) > 0:
        node, pulse_type, last_node = queue.pop(0)
        if pulse_type == "l":
            low_pulse += 1
        else:
            high_pulse += 1
        if node in end_values:
            # print("Output", pulse_type, "from", last_node)
            if pulse_type == "l":
                end_pulses += 1
            continue
        # print("Processing", node, pulse_type, "from", last_node)
        rule_info = rules_dict[node]
        if rule_info[0] == "b":
            for name in rule_info[1]:
                # print("b add", node, pulse_type, "->", name)
                queue.append((name, pulse_type, node))
        elif rule_info[0] == "&":
            state_dict_inv[node][last_node] = pulse_type
            new_pulse = "l" if all(
                [value == "h" for key, value in state_dict_inv[node].items()]) else "h"
            if new_pulse == "l":
                if node in interesting_nodes:
                    interesting_emmissions[node].append((i, new_pulse))
            for name in rule_info[1]:
                # print("% add", node, new_pulse, "->", name)
                queue.append((name, new_pulse, node))
        elif rule_info[0] == "%":
            if pulse_type == "l":
                cur = state_dict_ff[node]
                state_dict_ff[node] = "off" if cur == "on" else "on"
                new_pulse = "l" if cur == "on" else "h"
                for name in rule_info[1]:
                    # print("& add", node, new_pulse, "->", name)
                    queue.append((name, new_pulse, node))
        else:
            raise Exception("Invalid rule")
    print(i, end_pulses)
    if end_pulses == 1:
        print("Found", i)
        break

# changes = defaultdict(list)
# for node in interesting_nodes:
#     for i in range(len(sequence_of_states)-1):
#         for key, value in sequence_of_states[i][node].items():
#             if sequence_of_states[i+1][node][key] != value:
#                 changes[node+"_" +
#                         key].append((i+1, sequence_of_states[i][node][key]))

#         if sequence_of_states[i+1][node].items() != sequence_of_states[i][node].items():

#             print("Found change at", i, "for", node,
#                   sequence_of_states[i][node], sequence_of_states[i+1][node], sequence_of_states[i][node].items() != sequence_of_states[i+1][node].items())


# for key, value in changes.items():
#     print(key, value[:5])
#     steps_low = [value[i][0]for i in range(len(value)-1) if value[i][1] == "l"]
#     steps_high = [value[i][0]
#                   for i in range(len(value)-1) if value[i][1] == "h"]
#     set_diff_high = set(b-a for b, a in zip(steps_low[1:], steps_low[:-1]))
#     set_diff_low = set(b-a for b, a in zip(steps_high[1:], steps_high[:-1]))
#     print("Low", set_diff_low)
#     print("High", set_diff_high)
#     print("low", list(b-a for b, a in zip(steps_low[1:], steps_low[:-1])))
#     print("high", list(b-a for b, a in zip(steps_high[1:], steps_high[:-1])))

# print(interesting_emmissions["vc"])
steps = []
for node in interesting_nodes:
    val = [i for i, _ in interesting_emmissions[node]]

    print(node, val)
    print("dif:", list(b-a for b, a in zip(val[1:], val[:-1])))
    steps.append(val[1]-val[0])

print("lcm", math.lcm(*steps))
