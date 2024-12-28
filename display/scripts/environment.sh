#!/usr/bin/env bash

# Environment properties
# CNV_INC: any -I includes to pass to g++ for main and canvas
# CNV_SRC: which .cc file to use as canvas.h implementation
# CNV_LIB: any -L -l libraries to pass to g++ for main and canvas
# CNV_VER: canvas version used by preprocessor (see canvas.h)
# OUT_EXT: built program file extension. Set when building on Windows so Makefile only rebuilds when necessary.
# BST_INC: path to boost library
required_keys=('CNV_SRC' 'CNV_VER' 'BST_INC')



# this is assuming SFML is already in compiler lib path.
# may be helpful: g++ -E -v -xc++ /dev/null
# for TDM-GCC, copy SFML include/ and lib/ contents to 
# {TDM_GCC_INSTALLPATH}/{arch}/include and /lib
declare -A cfg_winsfml=(
   [CNV_INC]=''
   [CNV_SRC]='src/canvas/winsfml.cc'
   [CNV_LIB]='-lsfml-system -lsfml-window -lsfml-graphics -l:libwinmm.a -lws2_32 -lmswsock -lpthread' #last 3 required for boost -- add BST_LIB
   [CNV_VER]=1
   [OUT_EXT]='.exe'
   [BST_INC]='-Ilib/boostWindows/'
)

declare -A cfg_nixled=(
   [CNV_INC]='-Ilib/rpi-rgb-led-matrix/include'
   [CNV_SRC]='src/canvas/nixled.cc'
   [CNV_LIB]='-Llib/rpi-rgb-led-matrix/lib -lrgbmatrix -lrt -lm -lpthread'
   [CNV_VER]=2
   [BST_INC]='-Ilib/boostUnix/'
)

declare -A cfg_nixdummy=(
   [CNV_INC]=''
   [CNV_SRC]='src/canvas/nixdummy.cc'
)