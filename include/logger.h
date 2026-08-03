// Undefined log module protection
#ifndef LOG_MODULE
#warning "Undefined logger module"
#define LOG_MODULE "UNDEFINED"
#endif

#ifndef LOGGER_H
#define LOGGER_H

typedef enum {
  LOG_LVL_ERROR = 0,
  LOG_LVL_EXCEPT = 1,
  LOG_LVL_WARNING = 2,
  LOG_LVL_INFO = 3,
  LOG_LVL_DEBUG = 4,
} loglevel_e;

#define LOG_DEFAULT LOG_LVL_INFO

#define LOG_ERROR(fmt, ...) logger_error(LOG_MODULE, fmt, ##__VA_ARGS__)
#define LOG_EXCEPT(fmt, ...) logger_except(LOG_MODULE, fmt, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...) logger_warning(LOG_MODULE, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) logger_info(LOG_MODULE, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) logger_debug(LOG_MODULE, fmt, ##__VA_ARGS__)

loglevel_e logger_level_get(void);
void logger_level_set(loglevel_e level);

int logger_ansii_get(void);
void logger_ansii_set(int enabled);

void logger_error(const char *module, const char *fmt, ...)
    __attribute__((format(printf, 2, 3)));
void logger_except(const char *module, const char *fmt, ...)
    __attribute__((format(printf, 2, 3)));
void logger_warning(const char *module, const char *fmt, ...)
    __attribute__((format(printf, 2, 3)));
void logger_info(const char *module, const char *fmt, ...)
    __attribute__((format(printf, 2, 3)));
void logger_debug(const char *module, const char *fmt, ...)
    __attribute__((format(printf, 2, 3)));

#endif // LOGGER_H
