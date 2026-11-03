#!/bin/sh
# Records the catalog preview, asciiverse/blocks.cast, to hub/RECORDING.md
# (100x30) with hub's scripted recorder: level 1 at a person's pace, down
# the left shaft, pushing the first pair of blocks, jumping onto them for
# a sweet, knocking the upper blocks down, and pushing the fallen row of
# six along the bottom. Plays the packaged build
# (dist/blocks-<target>.tar.gz) with a throwaway HOME.
#
#   asciiverse/package.sh x86_64-unknown-linux-musl && asciiverse/record-preview.sh [castrec.py]
set -eu
cd "$(dirname "$0")/.."
rec=${1:-../../hub/tools/castrec.py}
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT
mkdir -p "$tmp/game" "$tmp/home"
tar -xzf dist/blocks-x86_64-unknown-linux-musl.tar.gz -C "$tmp/game"
python3 "$rec" asciiverse/blocks.cast "$tmp/game" \
  "w2.5;r16:l:0.25;w0.8;kx;w0.4;kl;w0.4;kx;w0.4;kl;w0.4;kl;w0.4;kx;w0.4;r6:l:0.3;w0.8;r14:l:0.25;w0.6;r12:l:0.25;w0.8;kk;w0.4;kk;w0.4;kx;w0.4;r4:l:0.35;w3" \
  -- env HOME="$tmp/home" ./block
