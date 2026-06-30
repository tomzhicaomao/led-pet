#!/usr/bin/env python3
"""Convert PNG sprite sheets to RGB565 C header arrays."""

import sys
from pathlib import Path
from PIL import Image


def rgb_to_565(r, g, b):
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def png_to_rgb565(png_path: Path, var_name: str, out_path: Path):
    img = Image.open(png_path).convert("RGBA")
    w, h = img.size
    pixels = []

    for y in range(h):
        for x in range(w):
            r, g, b, a = img.getpixel((x, y))
            if a < 128:
                pixels.append(0x0000)
            else:
                pixels.append(rgb_to_565(r, g, b))

    # Generate C header
    lines = [
        "// Auto-generated from {}".format(png_path.name),
        '#include <cstdint>',
        '',
        '#define {}_WIDTH  {}'.format(var_name.upper(), w),
        '#define {}_HEIGHT {}'.format(var_name.upper(), h),
        '',
        'const uint16_t {}[{}] PROGMEM = {{'.format(var_name, w * h),
    ]

    for i in range(0, len(pixels), 16):
        chunk = ', '.join('0x{:04X}'.format(p) for p in pixels[i:i+16])
        lines.append('    ' + chunk + (',' if i + 16 < len(pixels) else ''))

    lines.append('};')
    out_path.write_text('\n'.join(lines) + '\n')
    print("Wrote {}x{} sprite ({} bytes) to {}".format(w, h, w * h * 2, out_path))


if __name__ == '__main__':
    if len(sys.argv) != 4:
        print("Usage: png_to_rgb565.py <input.png> <var_name> <output.h>")
        sys.exit(1)

    png_to_rgb565(Path(sys.argv[1]), sys.argv[2], Path(sys.argv[3]))
