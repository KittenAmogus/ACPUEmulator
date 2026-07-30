### Tools
CC = gcc

### Flags

# Debug (unoptimized, debug things)
CFLAGS_DEBUG = -std=c11 -O0 -g3 -ggdb -Wall -Wextra -Wpedantic \
               -Wshadow -Wdouble-promotion -Wformat=2 \
               -fno-omit-frame-pointer -DDEBUG -lm
LDFLAGS_DEBUG = -fno-omit-frame-pointer -rdynamic

# Release (optimized, no debug things)
CFLAGS_RELEASE = -std=c11 -O3 -march=native -flto=auto -fomit-frame-pointer \
                 -ffast-math -ffunction-sections -fdata-sections \
                 -DNDEBUG -lm
LDFLAGS_RELEASE = -flto=auto -s -Wl,--gc-sections

# Default - debug
CFLAGS = $(CFLAGS_DEBUG)
LDFLAGS = $(LDFLAGS_DEBUG)

### Files
TARGET = Emulator

### Sources
CSOURCES = $(shell find src -name '*.c')
SOURCES = $(CSOURCES)

### Objects
OBJECTS = $(patsubst src/%,build/%.o,$(SOURCES))

### Rules
.PHONY: all clean release $(TARGET)

all: $(TARGET)

release: CFLAGS  = $(CFLAGS_RELEASE)
release: LDFLAGS = $(LDFLAGS_RELEASE)
release: clean $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(dir $@)
	@echo "(LD) ... => $(notdir $@)"
	$(CC) -o $@ $^ $(LDFLAGS) `sdl2-config --libs`

build/%.c.o: src/%.c
	@mkdir -p $(dir $@)
	@echo "(CC) $(notdir $<) => $(notdir $@)"
	$(CC) -o $@ -c $< $(CFLAGS) `sdl2-config --cflags`

clean:
	@rm -rf build $(TARGET)

