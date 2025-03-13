#!/bin/sh

GRIT=/opt/wonderful/thirdparty/blocksds/core/tools/grit/grit

# Process all .png files (for sprites)
for file in *.png; do
    # Skip files ending in .pngb
    if [[ "$file" == *.pngb ]]; then
        continue
    fi
    
    $GRIT "$file" -ftB -fh! -gTFF00FF -gt -gB8 -m!
done

# Move sprite files
mv *.pal *.img ../nitrofiles/sprite

# Process all .pngb files (for backgrounds)
for file in *.pngb; do
    $GRIT "$file" -ftB -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
done

# Move background files
mv *.pal *.img *.map ../nitrofiles/bg
#!/bin/sh

GRIT=/opt/wonderful/thirdparty/blocksds/core/tools/grit/grit

# Process all .png files (for sprites)
for file in *.png; do
    # Skip files ending in .pngb
    if [[ "$file" == *.pngb ]]; then
        continue
    fi
    
    $GRIT "$file" -ftB -fh! -gTFF00FF -gt -gB8 -m!
done

# Move sprite files
mv *.pal *.img ../nitrofiles/sprite

# Process all .pngb files (for backgrounds)
for file in *.pngb; do
    $GRIT "$file" -ftB -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
done

# Move background files
mv *.pal *.img *.map ../nitrofiles/bg
