#!/usr/bin/python3
from segtypes.n64.segment import N64Segment
import struct
from util import options
from n64img.image import RGBA16
import png

class N64SegSprite(N64Segment):
    def split(self, rom_bytes):
        #find sprite struct in the end
        struct_start = [self.rom_end - 0x44 - 4 * i for i in range(4)]
        for s in struct_start:
            if (rom_bytes[s + 0x34] & 0x80) and (rom_bytes[s + 0x38] & 0x80):
                start = s
                break
        
        width, height = struct.unpack_from(">hh", rom_bytes, start + 0x4)
        bitmaps_offset = struct.unpack_from(">I", rom_bytes, start + 0x34)[0] - self.vram_start + self.rom_start
        nbitmaps = struct.unpack_from(">hh", rom_bytes, start + 0x28)[0]

        rows = [[] for _ in range(height)]
        x, y = 0, 0

        for i in range(nbitmaps):
            tile_width = struct.unpack_from(">h", rom_bytes, bitmaps_offset + 0x10 * i + 0)[0]
            tile_height = struct.unpack_from(">h", rom_bytes, bitmaps_offset + 0x10 * i + 0xC)[0]
            tile_offset = struct.unpack_from(">I", rom_bytes, bitmaps_offset + 0x10 * i + 0x8)[0] - self.vram_start + self.rom_start
            tile_data = rom_bytes[tile_offset : tile_offset + tile_width * tile_height * 2]
            tile_image = RGBA16(tile_data, tile_width, tile_height).parse()
            for j in range(tile_height):
                if j % 2 == 0:
                    rows[y + j] += tile_image[4 * j * tile_width : 4 * (j + 1) * tile_width]
                else:
                    # shuffle image
                    for k in range(tile_width):
                        X = k ^ 2
                        rows[y + j] += tile_image[4 * j * tile_width + 4 * X : 4 * j * tile_width + 4 * X + 4]

            x += tile_width
            if x >= width:
                x = 0
                y += tile_height

        path = options.opts.asset_path / "sprite" / self.dir 
        path.mkdir(parents=True, exist_ok=True)
        with open(path / f"{self.name}.png", "wb") as f:
            png.Writer(width, height, greyscale = False, alpha = True).write(f, rows)
        