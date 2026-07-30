#define LOG_MODULE
#include <log.h>

#include <stdarg.h>
#include <stdio.h>

#define PCOL(file, col)                                                        \
  do {                                                                         \
    if (log_color) {                                                           \
      fprintf(file, col);                                                      \
    }                                                                          \
  } while (0)

#define PCLR(file) PCOL(file, "\x1b[0m")

static int log_color = 0;
static loglevel_e _level = LOG_DEFAULT;

loglevel_e log_getlvl(void) { return _level; }

int log_get_color(void) { return log_color; }

void log_setlvl(loglevel_e level) {
  _level =
      (level >= LOG_LVL_ERROR && level <= LOG_LVL_DEBUG ? level : LOG_DEFAULT);
}

void log_set_color(int enable) { log_color = enable; }

void log_error(const char *module, const char *fmt, ...) {
  if (_level < LOG_LVL_ERROR)
    return;
  PCOL(stderr, "\x1b[91;40;5m");
  if (log_color)
    fprintf(stderr, "\x1b[2K\r");

  fprintf(stderr, "[ERROR] %s: ", module);

  va_list args;
  va_start(args, fmt);

  vfprintf(stderr, fmt, args);

  va_end(args);
  PCLR(stderr);
  fprintf(stderr, "\r\n");
}

void log_except(const char *module, const char *fmt, ...) {
  if (_level < LOG_LVL_EXCEPT)
    return;
  PCOL(stderr, "\x1b[41;30m");
  if (log_color)
    fprintf(stderr, "\x1b[2K\r");

  fprintf(stderr, "[EXCEPTION] %s: ", module);

  va_list args;
  va_start(args, fmt);

  vfprintf(stderr, fmt, args);

  va_end(args);
  PCLR(stderr);
  fprintf(stderr, "\r\n");
}

void log_warning(const char *module, const char *fmt, ...) {
  if (_level < LOG_LVL_WARNING)
    return;
  PCOL(stdout, "\x1b[43;30m");
  if (log_color)
    fprintf(stdout, "\x1b[2K\r");

  fprintf(stdout, "[WARNING] %s: ", module);

  va_list args;
  va_start(args, fmt);

  vfprintf(stdout, fmt, args);

  va_end(args);
  PCLR(stdout);
  fprintf(stdout, "\r\n");
}

void log_info(const char *module, const char *fmt, ...) {
  if (_level < LOG_LVL_INFO)
    return;

  PCOL(stdout, "\x1b[0m");
  fprintf(stdout, "[INFO] %s: ", module);

  va_list args;
  va_start(args, fmt);

  vfprintf(stdout, fmt, args);

  va_end(args);
  PCLR(stdout);
  fprintf(stdout, "\r\n");
}

void log_debug(const char *module, const char *fmt, ...) {
  if (_level < LOG_LVL_DEBUG)
    return;
  PCOL(stdout, "\x1b[90m");

  fprintf(stdout, "[DEBUG] %s: ", module);

  va_list args;
  va_start(args, fmt);

  vfprintf(stdout, fmt, args);

  va_end(args);
  PCLR(stdout);
  fprintf(stdout, "\r\n");
}
