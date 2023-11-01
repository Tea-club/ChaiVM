import sys
import jinja2
import yaml
import os


def main() -> int:
    path_to_file = os.path.dirname(os.path.realpath(__file__)) + "/resources/instructions.yml"
    with open(path_to_file) as file:
        full = yaml.safe_load(file)
    instrs = full["instructions"]
    tpl = """There is ChaiVM's accumulator(acc) based ISA.

| Operation | Format | Description |
|:---------:|:------:|:------------|
{% for n, item in enumerate(items, 1) %}
| {{ item.mnemonic }} | {{ item.format }} | {{ item.description }} |
{% endfor %}

To generate this file use the following python script:
```shell
$ python3 tools/isa-spec.py
```
"""
    content = {
        'items': instrs,
        'enumerate': enumerate,
    }
    fp = open(os.path.dirname(os.path.realpath(__file__)) + "/../ISA.md", 'w')
    fp.write(
        jinja2
        .Template(tpl, trim_blocks=True)
        .render(content)
    )
    fp.close()
    return 0


if __name__ == '__main__':
    sys.exit(main())
