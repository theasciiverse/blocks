# Asciiverse changes

Blocks (`block`, with its level editor `bed`) by Marc Welz, with changes,
levels and the AT&T UNIX PC port by Ovis and the pyramid level and man
pages by Joey Hess. Released as block 0.6 at sunsite (games/strategy) on
24 March 1999, it was never packaged by a distribution, and it had no
repository until this one.

## History in this repository

1. `block 0.6` (tag `0.6`): the release tarball, unchanged:
   `block-0.6.tgz`, sha256
   `b4f1caf45a6f031a3ee322f1cb2408b85479013c9ad77c692828ae94aae060d8`,
   identical to ibiblio.org/pub/Linux/games/strategy/block-0.6.tgz (the
   only version left there). It includes 1999 i386 builds of `block` and
   `bed`, kept as they were; ours are built into `out/`.
2. Ours, below.

## Changes

- **Files in their places** (`paths.c`, `paths.h`): everything lived in
  `/usr/games/lib`, with a world-writable save folder. The game's levels
  are now found in `levels/` beside the binary, or `/usr/share/blocks`
  for system packages (`$BLOCKS_LEVELS` overrides); the high scores, the
  player's progress and levels made with `bed` live in
  `~/.local/share/blocks` (`$XDG_DATA_HOME/blocks`). A level named on the
  command line is looked for among the player's levels first, then the
  game's.
- **Builds and runs on today's systems**: prototypes for `loadnext`,
  `drawmob` and `drawsolid` and `static int count` (implicit declarations
  and implicit int are errors in current GCC); `ctype.h` for `isalpha`;
  built as gnu17 for its one K&R function.
- **Fixes**:
  - `speed=60000/baudrate()` divided by zero on a terminal that reports
    no baud rate.
  - `getlogin()` can be NULL, and `cuserid()` is gone from musl: the name
    in the high scores comes from `$USER`, `$LOGNAME`, `getlogin()` or
    "player", copied bounded into its 10 bytes.
  - The crumbling floor (`|`, `!`, `:`) was checked a row off at the
    bottom edge: the row below the player was taken modulo the width
    instead of the height.
  - After the last level the game looked for a seventh, then for a file
    called "default", and gave up with an error; it now says every level
    is done and starts again from the first.
  - The first frame shows the blocks and the player, not only after the
    first key.
- **The editor, bed**: it read the difficulty with `%d` straight into a
  one-byte field (writing past the level), copied the file name into 40
  bytes unchecked, and made each level as big as the window, past the
  126x126 grid on a wide one. New levels are now 78x23 like the game's
  own (so they play at 80x25), edited centred as the game draws them,
  loaded levels keep their size, and saves go to the player's levels
  folder under the name given.

## Building

`asciiverse/build.sh` (`STATIC=1` in Alpine for static musl binaries,
`CURSES=ncurses` on macOS) builds `out/block`, `out/bed` and `out/levels/`;
`asciiverse/package.sh <target>` makes the av archive;
`.github/workflows/release.yml` builds Linux x86_64 and aarch64 and macOS
on a `v<version>-av.<n>` tag, with .deb, .rpm, .apk and Arch packages
(`asciiverse/nfpm.yaml`).
