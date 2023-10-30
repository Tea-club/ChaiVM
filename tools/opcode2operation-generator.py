# @todo #8:90m Finish this script and genereate operations.hpp via yml file

import yaml
with open("resources/instructions.yml") as file:
    full = yaml.safe_load(file)

instructions = full["instructions"]
opcodes = set()
print(instructions)
opcodes_arr = ["Inv"] * 255

for instruction in instructions:
    print(instruction)
    old_len = len(opcodes)
    opcodes.add(instruction["fixedvalue"])
    new_len = len(opcodes)
    if(new_len == old_len):
        raise RuntimeError("opcode " + string(instruction["fixedvalue"]) + " repeats")
    opcodes_arr[instruction["fixedvalue"]] = instruction["mnemonic"]
print(max(opcodes))

for instruction in instructions:
    print(instruction["mnemonic"] + ",")

print("{", end=" ")
for operation in opcodes_arr:
    print(operation + ",", end=" ")
print("}")


print("Start templating")
import jinja2
tpl = """
{{ title }}
{{ '-' * title|length }}
{% for n, user in enumerate(users, 1) %}
{{ n }}. {{ user.name }} - должность: {{ user.status }}, оклад: ${{ user.salary }}
{% endfor %}
 """

content = {}

