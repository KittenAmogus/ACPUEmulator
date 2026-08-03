#ifndef PORTS_H
#define PORTS_H

#include <stdint.h>

#define PORT_MAX_PORTS 16

typedef uint8_t (*port_read_t)(uint8_t port);
typedef void (*port_write_t)(uint8_t port, uint8_t value);

typedef struct {
  uint8_t start;
  uint8_t end;
  port_read_t read;
  port_write_t write;
} port_t;

int port_register(uint8_t start, uint8_t end, port_read_t read,
                  port_write_t write);
int port_call_read(uint8_t port, uint8_t *value);
int port_call_write(uint8_t port, uint8_t value);

#endif // PORTS_H
