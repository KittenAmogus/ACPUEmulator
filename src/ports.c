#define LOG_MODULE "PORTS"
#include <logger.h>

#include <ports.h>

static port_t ports[PORT_MAX_PORTS];
static int ports_pos;

int port_register(uint8_t start, uint8_t end, port_read_t read,
                  port_write_t write) {
  if (ports_pos < 0 || ports_pos >= PORT_MAX_PORTS) {
    LOG_EXCEPT("Failed to register port: No memory");
    return -1;
  }

  port_t *pdata = (port_t *)&ports[ports_pos];
  pdata->start = start;
  pdata->end = end;
  pdata->read = read;
  pdata->write = write;

  LOG_DEBUG("Port registered: %02x - %02x", start, end);
  ++ports_pos;
  return ports_pos - 1;
}

int port_call_read(uint8_t port, uint8_t *value) {
  int read = 0;
  for (int i = 0; i < ports_pos; ++i) {
    port_t *pdata = (port_t *)&ports[i];

    // Check if in range
    if (pdata->start <= port && pdata->end >= port) {

      // Check if can read
      if (pdata->read && value) {
        *value = pdata->read(port);
        read = 1;
      }
    }
  }

  return read;
}

int port_call_write(uint8_t port, uint8_t value) {
  int write = 0;
  for (int i = 0; i < ports_pos; ++i) {
    port_t *pdata = (port_t *)&ports[i];

    // Check if in range
    if (pdata->start <= port && pdata->end >= port) {

      // Check if can write
      if (pdata->write) {
        pdata->write(port, value);
        write = 1;
      }
    }
  }

  return write;
}
