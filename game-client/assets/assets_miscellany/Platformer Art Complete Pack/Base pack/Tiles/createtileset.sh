#!/bin/sh
rm -f *.png
convert castle/*.png +append castle.png
convert dirt/*.png +append dirt.png
convert grass/*.png +append grass.png
convert sand/*.png +append sand.png
convert snow/*.png +append snow.png
convert stone/*.png +append stone.png
convert junk/1/*.png +append junk1.png
convert junk/*.png +append junk2.png
convert *.png -append dungeon.png
