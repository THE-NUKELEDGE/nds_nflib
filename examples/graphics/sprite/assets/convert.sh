#!/bin/sh

GRIT=/opt/wonderful/thirdparty/blocksds/core/tools/grit/grit


$GRIT mario.png -ftB -fh! -gTFF00FF -gt -gB8 -m!
$GRIT character.png -ftB -fh! -gTFF00FF -gt -gB8 -m!

mv *.pal *.img ../nitrofiles/sprite

$GRIT nfl.png -ftB -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
$GRIT bg2.png -ftB -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
$GRIT bg3.png -ftB -fh! -gTFF00FF -gt -gB8 -mR8 -mLs

mv *.pal *.img *.map ../nitrofiles/bg
