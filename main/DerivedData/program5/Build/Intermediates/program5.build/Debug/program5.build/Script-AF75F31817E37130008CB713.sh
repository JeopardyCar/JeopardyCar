#!/bin/sh
currentDir=`pwd`
source=$currentDir/Shaders/
dest=$BUILT_PRODUCTS_DIR/Shaders/
if [ ! -d $dest ]; then
    mkdir $dest
fi
cp "$source/"*vert "$dest"
cp "$source/"*frag "$dest"
