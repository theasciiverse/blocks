/* Asciiverse: see paths.h. */
#include "paths.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#ifdef __APPLE__
#include <stdint.h>
#include <mach-o/dyld.h>
#endif

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

static int exists(const char *p) {
  struct stat st;
  return stat(p, &st) == 0;
}

static int is_dir(const char *p) {
  struct stat st;
  return stat(p, &st) == 0 && S_ISDIR(st.st_mode);
}

static void with_slash(char *p, size_t size) {
  size_t n = strlen(p);
  if (n && p[n - 1] != '/' && n + 1 < size) {
    p[n] = '/';
    p[n + 1] = '\0';
  }
}

/* the folder holding the running executable, or "" */
static void exe_dir(char *out, size_t size) {
  char buf[PATH_MAX];
  char *slash;
  out[0] = '\0';
#ifdef __APPLE__
  uint32_t n = sizeof buf;
  if (_NSGetExecutablePath(buf, &n) != 0) return;
  {
    char real[PATH_MAX];
    if (realpath(buf, real)) snprintf(buf, sizeof buf, "%s", real);
  }
#else
  ssize_t n = readlink("/proc/self/exe", buf, sizeof buf - 1);
  if (n <= 0) return;
  buf[n] = '\0';
#endif
  slash = strrchr(buf, '/');
  if (!slash) return;
  *slash = '\0';
  snprintf(out, size, "%s", buf);
}

const char *levels_dir(void) {
  static char dir[PATH_MAX];
  char exe[PATH_MAX], try[PATH_MAX];
  const char *env;
  if (dir[0]) return dir;
  if ((env = getenv("BLOCKS_LEVELS")) && *env) {
    snprintf(dir, sizeof dir, "%s", env);
  } else {
    exe_dir(exe, sizeof exe);
    snprintf(try, sizeof try, "%s/levels", exe);
    if (exe[0] && is_dir(try)) snprintf(dir, sizeof dir, "%s", try);
    else if (is_dir("/usr/share/blocks")) snprintf(dir, sizeof dir, "/usr/share/blocks");
    else snprintf(dir, sizeof dir, "./levels");
  }
  with_slash(dir, sizeof dir);
  return dir;
}

static void make_dirs(char *p) {
  char *s;
  for (s = p + 1; *s; s++) {
    if (*s == '/') {
      *s = '\0';
      mkdir(p, 0755);
      *s = '/';
    }
  }
  mkdir(p, 0755);
}

const char *user_dir(void) {
  static char dir[PATH_MAX];
  char levels[PATH_MAX];
  const char *env;
  if (dir[0]) return dir;
  if ((env = getenv("XDG_DATA_HOME")) && *env)
    snprintf(dir, sizeof dir, "%s/blocks", env);
  else if ((env = getenv("HOME")) && *env)
    snprintf(dir, sizeof dir, "%s/.local/share/blocks", env);
  else
    snprintf(dir, sizeof dir, ".");
  make_dirs(dir);
  snprintf(levels, sizeof levels, "%s/levels", dir);
  mkdir(levels, 0755);
  with_slash(dir, sizeof dir);
  return dir;
}

void find_level(char *out, size_t size, const char *name) {
  char first[PATH_MAX];
  if (strchr(name, '/')) {
    snprintf(out, size, "%s", name);
    return;
  }
  snprintf(first, sizeof first, "%slevels/%s.block", user_dir(), name);
  if (exists(first)) { snprintf(out, size, "%s", first); return; }
  snprintf(out, size, "%s%s.block", levels_dir(), name);
  if (exists(out)) return;
  snprintf(out, size, "%s.block", name);
  if (exists(out)) return;
  snprintf(out, size, "%s", name);
  if (exists(out)) return;
  snprintf(out, size, "%s", first);
}

const char *player_name(void) {
  const char *n;
  if ((n = getenv("USER")) && *n) return n;
  if ((n = getenv("LOGNAME")) && *n) return n;
  if ((n = getlogin()) && *n) return n;
  return "player";
}
