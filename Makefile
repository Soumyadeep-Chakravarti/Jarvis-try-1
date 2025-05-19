CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -g -MMD -MP
# -MMD: generate dependency files excluding system headers
# -MP: add phony targets for dependencies to avoid errors on deleted headers

SRC := $(shell find src -name '*.c')
OBJ := $(patsubst src/%.c, build/%.o, $(SRC))
DEP := $(OBJ:.o=.d)
TARGET = build/jarvis

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	@echo -e "\033[1;32mLinking $@\033[0m"
	$(CC) $(CFLAGS) $^ -o $@

build/%.o: src/%.c | build
	@echo -e "\033[1;34mCompiling $<\033[0m"
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build

# Include dependency files if they exist
-include $(DEP)

clean:
	@echo -e "\033[1;31mCleaning build files...\033[0m"
	rm -rf build/*
