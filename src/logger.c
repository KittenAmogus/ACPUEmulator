#define LOG_MODULE "LOGGER"
#include <logger.h>

#include <stdarg.h>
#include <stdio.h>

static const char *level_strings[] = {
    [LOG_LVL_ERROR] = "ERROR    ",   [LOG_LVL_EXCEPT] = "EXCEPTION",
    [LOG_LVL_WARNING] = "WARNING  ", [LOG_LVL_INFO] = "INFO     ",
    [LOG_LVL_DEBUG] = "DEBUG    ",
};

static const char *level_colors[] = {
    [LOG_LVL_ERROR] = "\x1b[31;40;5m\x1b[2K\r",
    [LOG_LVL_EXCEPT] = "\x1b[30;41m\x1b[2K\r",
    [LOG_LVL_WARNING] = "\x1b[30;43m\x1b[2K\r",
    [LOG_LVL_INFO] = "\x1b[0m",
    [LOG_LVL_DEBUG] = "\x1b[90m\r",
};

static int log_enable_ansii = 0;
static int log_loglevel = LOG_DEFAULT;

static void print_message(loglevel_e lvl, const char *module, const char *fmt,
                          va_list args) __attribute__((format(printf, 3, 0)));
static void print_message(loglevel_e lvl, const char *module, const char *fmt,
                          va_list args) {
  // Get prefix and color
  const char *pref = level_strings[lvl];
  const char *color = level_colors[lvl];

  // Get file descriptor
  FILE *file = (lvl <= LOG_LVL_EXCEPT ? stderr : stdout);

  // Enable color
  if (log_enable_ansii) {
    fprintf(file, "%s", color);
  }

  // Print prefix
  fprintf(file, "[%s] (%s): ", pref, module);

  // Print va_list
  vfprintf(file, fmt, args);

  // Disable color
  if (log_enable_ansii) {
    fprintf(file, "%s", "\x1b[0m\r\n");
  } else {
    fprintf(file, "%s", "\r\n");
  }

  // Flush
  fflush(file);
}

int logger_ansii_get(void) { return log_enable_ansii; }
void logger_ansii_set(int enabled) { log_enable_ansii = enabled; }

loglevel_e logger_level_get(void) { return log_loglevel; }
void logger_level_set(loglevel_e level) { log_loglevel = level; }

void logger_error(const char *module, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  print_message(LOG_LVL_ERROR, module, fmt, args);
  va_end(args);
}

void logger_except(const char *module, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  print_message(LOG_LVL_EXCEPT, module, fmt, args);
  va_end(args);
}

void logger_warning(const char *module, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  print_message(LOG_LVL_WARNING, module, fmt, args);
  va_end(args);
}

void logger_info(const char *module, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  print_message(LOG_LVL_INFO, module, fmt, args);
  va_end(args);
}

void logger_debug(const char *module, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  print_message(LOG_LVL_DEBUG, module, fmt, args);
  va_end(args);
}
