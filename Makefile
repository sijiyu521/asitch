# Asitch — C / SDL2 构建脚本

CC      ?= cc
AR      ?= ar
CFLAGS  ?= -O2
CFLAGS  += -std=c11 -Wall -Wextra -Iinclude $(shell sdl2-config --cflags)
LDLIBS  += $(shell sdl2-config --libs) -lSDL2_image -lm

BUILD   := build
LIB     := $(BUILD)/libasitch.a
SRCS    := $(wildcard src/*.c)
OBJS    := $(SRCS:src/%.c=$(BUILD)/%.o)
DEMO    := $(BUILD)/minimal-desktop

all: $(LIB) $(DEMO)

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/%.o: src/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB): $(OBJS)
	$(AR) rcs $@ $^

$(DEMO): examples/minimal-desktop/main.c $(LIB) | $(BUILD)
	$(CC) $(CFLAGS) $< -o $@ $(LIB) $(LDLIBS)

run: $(DEMO)
	./$(DEMO)

clean:
	rm -rf $(BUILD)

.PHONY: all run clean
