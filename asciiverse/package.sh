#!/bin/sh
# Packs Blocks into its archive for av: dist/blocks-<target>.tar.gz with
# block, the level editor bed, levels/ (1.block to 6.block), the man pages,
# README and PATCHES.md.
#
#   asciiverse/package.sh x86_64-unknown-linux-musl
set -eu
cd "$(dirname "$0")/.."
target="$1"
stage="stage/blocks"
rm -rf "$stage" && mkdir -p "$stage" dist
cp out/block out/bed README PATCHES.md block.6 bed.6 "$stage/"
cp -r out/levels "$stage/levels"
tar --owner=0 --group=0 --numeric-owner -czf "dist/blocks-$target.tar.gz" -C "$stage" . 2>/dev/null \
  || tar -czf "dist/blocks-$target.tar.gz" -C "$stage" .
ls -l dist
