from collections import defaultdict
import os


source_file_dir = os.path.dirname(os.path.abspath(__file__))

rule_broadcast = 0
rule_inverter = 1
rule_flipflop = 2

rules_dict = {}

state_dict_inv = defaultdict(dict)
state_dict_ff = defaultdict(lambda: "off")
end_values = []
with open(source_file_dir+"/"+"final.txt", "r", encoding="utf8") as f:

    for line in f:
        print(line.strip())

        p = line.split(" -> ")

        name = p[0] if p[0][0] == "b" else p[0][1:]
        print(p[1].split(","))
        rules_dict[name] = (p[0][0], [x.strip() for x in p[1].split(",")])
        rule, recievers = rules_dict[name]
        for reciever in recievers:
            print(rule, reciever)

            state_dict_inv[reciever][name] = "l"


print(rules_dict)

for name, (rule, receivers) in rules_dict.items():
    for reciever in receivers:
        if reciever not in rules_dict:
            print("end node", reciever)
            end_values.append(reciever)

input("Press enter to continue")

low_pulse = 0
high_pulse = 0

for i in range(100000000):
    queue = [("broadcaster", "l", None)]
    end_pulses = 0
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
print(low_pulse, high_pulse)
print(low_pulse*high_pulse)
