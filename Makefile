CC := gcc
CFLAGS := -Wall -Wextra -g -Isrc
LDFLAGS := -lutil

SRC_DIR := src
BUILD_DIR := build

# Find all .c files in src and its subdirectories
SRCS := $(shell find $(SRC_DIR) -name "*.c")
# Convert .c files to .o files in the build directory
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

TARGET := typhoon

.PHONY: all clean rebuild

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Rule to compile .c files to .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

rebuild: clean all
