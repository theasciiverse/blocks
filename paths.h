/* Asciiverse: where block and bed find the levels and keep the player's
   files, instead of the fixed /usr/games/lib of 1999. */
#ifndef BLOCKS_PATHS_H
#define BLOCKS_PATHS_H

#include <stddef.h>

/* The game's own levels (1.block, 2.block, ...), with a trailing '/':
   $BLOCKS_LEVELS if set, else levels/ beside the executable, else
   /usr/share/blocks/, else ./levels/. */
const char *levels_dir(void);

/* The player's folder, with a trailing '/', created if missing:
   $XDG_DATA_HOME/blocks or ~/.local/share/blocks. Holds score (the high
   scores), progress (the next level to play) and levels/ (levels made
   with bed). */
const char *user_dir(void);

/* A level by name: a path with a '/' as given; otherwise
   <user_dir>/levels/<name>.block, then <levels_dir>/<name>.block, then
   <name>.block and <name> in the current folder. Writes the first that
   exists (or the user_dir one if none does) to out. */
void find_level(char *out, size_t size, const char *name);

/* The name to put in the high scores: $USER, $LOGNAME, getlogin(), or
   "player". */
const char *player_name(void);

#endif
