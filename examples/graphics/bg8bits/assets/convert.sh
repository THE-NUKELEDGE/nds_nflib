#!/bin/sh

# The two backgrounds that share the palette have been edited to use 128 colors
# each, so that the final combined palette is 256 colors in size.

grit img8b_1.png img8b_2.png -ftb -fh! -gTFF00FF -gb -gB8 -pS -Oimg8b_1.pal

# This image has been edited so that it uses 255 colors and the last one is
# free. That way we can set it to red manually and paint using it.

grit img8b_3.png -ftb -fh! -gTFF00FF -gb -gB8

for file in *.bin; do
    mv -- "$file" "${file%.bin}"
done

# This is required or the library will say that the second background doesn't
# have a palette.

cp img8b_1.pal img8b_2.pal

mv *.pal *.img ../nitrofiles/bmp
