# @todo #8:90m Finish this script and genereate operations.hpp via yml file

import yaml
with open("resources/instructions.yml") as file:
    full = yaml.safe_load(file)

instructions = full["instructions"]
opcodes = list()
print(instructions)
opcodes_arr = ["Inv"] * 255

for instruction in instructions:
    print(instruction)
    opcodes.append(instruction["fixedvalue"])
    opcodes_arr[instruction["fixedvalue"]] = instruction["mnemonic"]
print(max(opcodes))

for instruction in instructions:
    print(instruction["mnemonic"] + ",")

print("{", end=" ")
for operation in opcodes_arr:
    print(operation + ",", end=" ")
print("}")
