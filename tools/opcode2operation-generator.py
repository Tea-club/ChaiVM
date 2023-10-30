import sys
from datetime import datetime
import jinja2
import yaml


def main() -> int:
    if len(sys.argv) != 2:
        raise RuntimeError('Should be 2 args, received ' + str(len(sys.argv)))
    with open("resources/instructions.yml") as file:
        full = yaml.safe_load(file)
    instructions = full["instructions"]
    opcodes = set()
    # print(instructions)
    opcodes_arr = ["Inv"] * 255

    # Validating yaml file
    for instruction in instructions:
        # print(instruction)
        old_len = len(opcodes)
        opcodes.add(instruction["fixedvalue"])
        new_len = len(opcodes)
        if (new_len == old_len):
            raise RuntimeError("opcode " + str(instruction["fixedvalue"]) + " repeats")
        opcodes_arr[instruction["fixedvalue"]] = instruction["mnemonic"]
    # print(max(opcodes))
    tpl = """{{ disclaimer }}
#pragma once
namespace chai::interpreter {
enum Operation {
{% for n, item in enumerate(items, 1) %}
    {{ item.mnemonic }} = {{ item.fixedvalue }},
{% endfor %}
};
} // namespace chai::interpreter

"""
    content = {
        'disclaimer': '/* This file was automatically generated by the script ' +
            sys.argv[0] + ' at ' + datetime.now().strftime("%d.%m.%Y %H:%M:%S") + ' */',
        'items': instructions,
        'enumerate': enumerate,
    }

    fp = open(sys.argv[1], 'w')
    fp.write(
        jinja2
        .Template(tpl, trim_blocks=True)
        .render(content)
    )
    fp.close()
    return 0


if __name__ == '__main__':
    sys.exit(main())
