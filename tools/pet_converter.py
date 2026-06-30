#!/usr/bin/env python3
"""
LED Pet Data Converter - Convert pixel designs to C++ sprite data.

Usage:
    python tools/pet_converter.py grid examples/dragon.txt --name "冰晶龙"
    python tools/pet_converter.py validate data/pets/dragon.json
    python tools/pet_converter.py convert pet_design.json -o pet_data.h

Pixel grid format (32 columns x 8 rows):
    Each row is 32 characters, '1' = LED on, '0' = LED off.
    Separate animation frames with blank lines.
"""

import json, sys, os
from typing import List, Dict

FB_WIDTH = 32
FB_HEIGHT = 8


def parse_pixel_grid(text: str) -> List[int]:
    lines = [l.strip() for l in text.strip().split('\n') if l.strip()]
    lines = lines[:FB_HEIGHT]
    columns = []
    for col in range(FB_WIDTH):
        byte_val = 0
        for row in range(FB_HEIGHT):
            if row < len(lines) and col < len(lines[row]) and lines[row][col] == '1':
                byte_val |= (1 << (7 - row))
        columns.append(byte_val)
    return columns


def bytes_to_c_array(data: List[int], name: str) -> str:
    hex_values = ', '.join(f'0x{v:02X}' for v in data)
    return f'const uint8_t {name}[FB_WIDTH] = {{\n    {hex_values}\n}};'


def generate_pet_header(pet_json: Dict) -> str:
    lines = []
    lines.append('#pragma once')
    lines.append('#include "pet_data.h"')
    lines.append('')
    pet_id = pet_json.get('petId', 'custom_pet')
    display_name = pet_json.get('displayName', 'Custom Pet')
    for si, stage in enumerate(pet_json.get('stages', [])):
        for ai, anim in enumerate(stage.get('animations', [])):
            for fi, frame_grid in enumerate(anim.get('frames', [])):
                data = parse_pixel_grid(frame_grid)
                lines.append(bytes_to_c_array(data, f'{pet_id}_s{si}_a{ai}_f{fi}'))
                lines.append('')
            fc = len(anim.get('frames', []))
            refs = ', '.join(f'{{{pet_id}_s{si}_a{ai}_f{i}, {anim.get("frameDurationMs", 200)}}}' for i in range(fc))
            lines.append(f'const Frame {pet_id}_s{si}_a{ai}_frames[] = {{\n    {refs},\n}};')
            lines.append('')
    print(f"Generated pet: {display_name} ({pet_id})")
    print(f"  Stages: {len(pet_json.get('stages', []))}")
    return '\n'.join(lines)


def validate_pet_json(data: Dict) -> List[str]:
    errors = []
    if 'petId' not in data: errors.append("Missing 'petId'")
    if 'displayName' not in data: errors.append("Missing 'displayName'")
    stages = data.get('stages', [])
    if len(stages) != 5: errors.append(f"Expected 5 stages, got {len(stages)}")
    for i, stage in enumerate(stages):
        anims = stage.get('animations', [])
        if len(anims) != 4: errors.append(f"Stage {i}: expected 4 animations, got {len(anims)}")
        for j, anim in enumerate(anims):
            for k, frame in enumerate(anim.get('frames', [])):
                rows = [r.strip() for r in frame.strip().split('\n') if r.strip()]
                if len(rows) != FB_HEIGHT: errors.append(f"S{i} A{j} F{k}: expected {FB_HEIGHT} rows, got {len(rows)}")
                for r, row in enumerate(rows):
                    if len(row) != FB_WIDTH: errors.append(f"S{i} A{j} F{k} R{r}: expected {FB_WIDTH} cols, got {len(row)}")
    return errors


def main():
    if len(sys.argv) < 2: print(__doc__); sys.exit(1)
    cmd = sys.argv[1]

    if cmd == 'validate':
        with open(sys.argv[2]) as f: data = json.load(f)
        errors = validate_pet_json(data)
        if errors: print("FAILED:\n" + '\n'.join(f'  - {e}' for e in errors)); sys.exit(1)
        print("Validation PASSED!")

    elif cmd == 'convert':
        with open(sys.argv[2]) as f: data = json.load(f)
        errors = validate_pet_json(data)
        if errors: print("FAILED:\n" + '\n'.join(f'  - {e}' for e in errors)); sys.exit(1)
        header = generate_pet_header(data)
        output_path = None
        if '-o' in sys.argv:
            idx = sys.argv.index('-o')
            if idx + 1 < len(sys.argv): output_path = sys.argv[idx + 1]
        if output_path:
            with open(output_path, 'w') as f: f.write(header)
            print(f"Written to {output_path}")
        else:
            print(header)

    elif cmd == 'grid':
        with open(sys.argv[2]) as f: text = f.read()
        data = parse_pixel_grid(text)
        name = "custom_frame"
        if '--name' in sys.argv:
            idx = sys.argv.index('--name')
            if idx + 1 < len(sys.argv): name = sys.argv[idx + 1]
        print(bytes_to_c_array(data, name))

    else: print(f"Unknown command: {cmd}"); sys.exit(1)


if __name__ == '__main__':
    main()
