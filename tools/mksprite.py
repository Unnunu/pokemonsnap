#!/usr/bin/python3
import sys
import png
from pathlib import Path
import os

def choose_tile_size(width, height):
    # TODO
    tw, th = 32, 64
    if height <= 140:
        th = 32
    
    tw = min(tw, width)
    th = min(th, height)
    return tw, th

def pack_color(r, g, b, a):
    r = r >> 3
    g = g >> 3
    b = b >> 3
    a = a >> 7

    return (r << 11) | (g << 6) | (b << 1) | a

def write_array(rows, x, y, width, height, name, index, file):
    file.write(f"static Gfx {name}_bm{index}_dummy_aligner[] = {{ gsSPEndDisplayList() }};\n\n")
    file.write(f"unsigned short {name}_bm{index}[] = {{\n")
    for row in range(y, y + height):
        file.write("    ")
        for col in range(x, x + width):
            if row % 2 == 1:
                col ^= 2
            r, g, b, a = rows[row][4 * col : 4 * (col + 1)]
            file.write(f"{hex(pack_color(r, g, b, a))}, ")
        file.write("\n")
    file.write("};\n\n")

def main(path, build_path):
    width, height, rows, _ = png.Reader(path).read()
    rows = list(rows)
    tile_width, tile_height = choose_tile_size(width, height)

    Path(build_path).parent.mkdir(parents=True, exist_ok=True)
    name = "_".join(Path(path).parts[1:])[:-4]
    with open(build_path+".inc.c", "w") as f:
        f.write("#include <PR/sp.h>\n\n")

        #write image arrays
        x, y, index = 0, 0, 0
        bitmaps = []
        while y < height:
            cur_width = min(tile_width, width - x)
            cur_height = min(tile_height, height - y)
            bitmaps.append((cur_width, cur_height, index))
            write_array(rows, x, y, cur_width, cur_height, name, index, f)

            index += 1
            x += cur_width
            if x >= width:
                x = 0
                y += cur_height

        #write bitmap structs
        f.write(f"static Bitmap {name}_bitmaps[] = {{\n")
        for b in bitmaps:
            f.write(f"    {{ {b[0]}, {b[0]}, 0, 0, {name}_bm{b[2]}, {b[1]}, 0 }},\n")
        f.write("};\n\n")

        #write Sprite struct
        f.write(f"#define NUM_{name}_BMS (sizeof({name}_bitmaps)/sizeof(Bitmap))\n\n")
        f.write(f"static Gfx {name}_dl[NUM_DL(NUM_{name}_BMS)];\n\n")
        f.write(f"Sprite {name} = {{\n")
        f.write("    0, 0,\n")
        f.write(f"    {width}, {height},\n")
        f.write("    1.0, 1.0,\n")
        f.write("    0, 0,\n")
        f.write("    SP_TEXSHUF | SP_Z | SP_FASTCOPY,\n")
        f.write("    0x1234,\n")
        f.write("    255, 255, 255, 255,\n")
        f.write("    0, 0, NULL,\n")
        f.write("    0, 1,\n")
        f.write(f"    NUM_{name}_BMS,\n")
        f.write(f"    NUM_DL(NUM_{name}_BMS),\n")
        f.write(f"    {tile_height}, {tile_height},\n")
        f.write("    G_IM_FMT_RGBA,\n")
        f.write("    G_IM_SIZ_16b,\n")
        f.write(f"    {name}_bitmaps,\n")
        f.write(f"    {name}_dl,\n")
        f.write("    NULL,\n")
        f.write("    0, 0\n")
        f.write("};\n")

if __name__ == '__main__':
    main(sys.argv[1], sys.argv[2])
