#ifndef LOG_MODULE
#warning "Undefined log module"
#define LOG_MODULE "UNDEF"
#endif

#ifndef LOG_H
#define LOG_H

typedef enum {
  LOG_LVL_ERROR = 0,
  LOG_LVL_EXCEPT = 1,
  LOG_LVL_WARNING = 2,
  LOG_LVL_INFO = 3,
  LOG_LVL_DEBUG = 4,
} loglevel_e;

#define LOG_DEFAULT LOG_LVL_INFO

#define LOG_ERROR(fmt, ...) log_error(LOG_MODULE, fmt, ##__VA_ARGS__)
#define LOG_EXCEPT(fmt, ...) log_except(LOG_MODULE, fmt, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...) log_warning(LOG_MODULE, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) log_info(LOG_MODULE, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) log_debug(LOG_MODULE, fmt, ##__VA_ARGS__)

loglevel_e log_getlvl(void);
void log_setlvl(loglevel_e level);

void log_error(const char *module, const char *fmt, ...);
void log_except(const char *module, const char *fmt, ...);
void log_warning(const char *module, const char *fmt, ...);
void log_info(const char *module, const char *fmt, ...);
void log_debug(const char *module, const char *fmt, ...);

#endif // LOG_H
