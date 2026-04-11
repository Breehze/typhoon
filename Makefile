CC := gcc
CFLAGS := -Wall -Wextra -g -Isrc -fsanitize=address -fsanitize=undefined -fsanitize=leak
LDFLAGS := -lutil -fsanitize=address,undefined,leak

SRC_DIR := src
BUILD_DIR := build

SRCS := $(shell find $(SRC_DIR) -name "*.c")

OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

TARGET := typhoon

.PHONY: all clean rebuild

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

rebuild: clean all
