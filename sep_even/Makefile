CC      := gcc
PKGS    :=

CFLAGS  := -Wall -Wextra -g -O0 -std=c11 -Iinclude -MMD -MP
LDFLAGS :=
LDLIBS  :=

ifneq ($(PKGS),)
CFLAGS  += $(shell pkg-config --cflags $(PKGS))
LDLIBS  += $(shell pkg-config --libs $(PKGS))
endif

SRC     := $(shell find src -type f -name '*.c' 2>/dev/null)
OBJ     := $(patsubst src/%.c,build/obj/%.o,$(SRC))
DEP     := $(OBJ:.o=.d)
TARGET  := build/app

.PHONY: all run clean compdb

all: $(TARGET)

$(TARGET): $(OBJ)
	mkdir -p $(dir $@)
	$(CC) $(OBJ) -o $@ $(LDFLAGS) $(LDLIBS)

build/obj/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf build

compdb:
	rm -rf build
	bear -- $(MAKE) all

-include $(DEP)
