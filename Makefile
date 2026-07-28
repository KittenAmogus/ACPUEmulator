CC = gcc

CFLAGS = -march=native -O2 -Wall -Wextra -Wno-unused-parameter \
				 -isystem include -g3 `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lm

TARGET = Emulator

CSOURCES = $(shell find src -name '*.c')
SOURCES = $(CSOURCES)

OBJECTS = $(patsubst src/%,build/%.o,$(CSOURCES))

.PHONY: all clean $(TARGET)

all: $(TARGET)


$(TARGET): $(OBJECTS)
	@echo "(LD) ... => $(notdir $@)"
	$(CC) $(LDFLAGS) -o $@ $^

build/%.c.o: src/%.c | $(GLAD_FILES)
	@mkdir -p $(dir $@)
	@echo "(CC) $(notdir $<) => $(notdir $@)"
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	@rm -rf build $(TARGET)

