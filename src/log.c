#define LOG_MODULE
#include <log.h>

#include <stdarg.h>
#include <stdio.h>

static loglevel_e _level = LOG_DEFAULT;

loglevel_e log_getlvl(void) { return _level; }

void log_setlvl(loglevel_e level) {
  _level =
      (level >= LOG_LVL_ERROR && level <= LOG_LVL_DEBUG ? level : LOG_DEFAULT);
}

void log_error(const char *module, const char *fmt, ...) {
  if (_level < LOG_LVL_ERROR)
    return;

  fprintf(stderr, "[ERROR] %s: ", module);

  va_list args;
  va_start(args, fmt);

  vfprintf(stderr, fmt, args);

  va_end(args);
  fprintf(stderr, "\r\n");
}

void log_except(const char *module, const char *fmt, ...) {
  if (_level < LOG_LVL_EXCEPT)
    return;
  fprintf(stderr, "[EXCEPTION] %s: ", module);

  va_list args;
  va_start(args, fmt);

  vfprintf(stderr, fmt, args);

  va_end(args);
  fprintf(stderr, "\r\n");
}

void log_warning(const char *module, const char *fmt, ...) {
  if (_level < LOG_LVL_WARNING)
    return;

  fprintf(stdout, "[WARNING] %s: ", module);

  va_list args;
  va_start(args, fmt);

  vfprintf(stdout, fmt, args);

  va_end(args);
  fprintf(stdout, "\r\n");
}

void log_info(const char *module, const char *fmt, ...) {
  if (_level < LOG_LVL_INFO)
    return;

  fprintf(stdout, "[INFO] %s: ", module);

  va_list args;
  va_start(args, fmt);

  vfprintf(stdout, fmt, args);

  va_end(args);
  fprintf(stdout, "\r\n");
}

void log_debug(const char *module, const char *fmt, ...) {
  if (_level < LOG_LVL_DEBUG)
    return;

  fprintf(stdout, "[DEBUG] %s: ", module);

  va_list args;
  va_start(args, fmt);

  vfprintf(stdout, fmt, args);

  va_end(args);
  fprintf(stdout, "\r\n");
}
