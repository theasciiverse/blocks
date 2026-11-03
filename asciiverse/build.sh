#!/bin/sh
# Builds the game and its level editor into out/: out/block, out/bed and
# out/levels/ (the six levels, 1.block to 6.block), laid out the way the
# av archive and paths.c expect. C as of 1999 (one K&R function), so it
# is compiled as gnu17, not today's default C23.
#
#   asciiverse/build.sh                # Linux (ncursesw)
#   STATIC=1 asciiverse/build.sh       # Alpine: static musl binaries
#   CURSES=ncurses asciiverse/build.sh # macOS (the system's ncurses)
set -eu
cd "$(dirname "$0")/.."
CC=${CC:-cc}
flags="-std=gnu17 -O2 -Wall -Wno-unused -Wno-parentheses -Wno-implicit-fallthrough -Wno-sequence-point -Wno-char-subscripts -Wno-format-overflow -Wno-format-truncation -DUCOL -DNEW"
libs="-l${CURSES:-ncursesw}"
[ -n "${STATIC:-}" ] && libs="-static -s $libs"
rm -rf out && mkdir -p out/levels
$CC $flags -o out/block block.c paths.c $libs
$CC $flags -o out/bed bed.c paths.c $libs
cp 1.block 2.block 3.block 4.block 5.block 6.block out/levels/
echo "built block and bed"
