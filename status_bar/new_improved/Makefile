CC ?= clang
INCS := -Iinclude
LDFLAGS := $(shell pkg-config --libs xcb xcb-xkb alsa)
CFLAGS := -std=c99 -pedantic -Wall -Wextra -D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE $(INCS) $(CFLAGS)

BUILD_DIR ?= build
BUILD_TYPE ?= debug

ifeq (release,$(BUILD_TYPE))
	CFLAGS += -O2
else ifeq (debug,$(BUILD_TYPE))
	CFLAGS += -g -Og
endif

OUT_DIR ?= $(BUILD_DIR)/$(BUILD_TYPE)
OUT_FILE ?= out

SRC_DIR ?= src
SRC_FILE_MASK := $(SRC_DIR)/%.c
SRCS := $(shell find $(SRC_DIR) -name "*.c")

OBJ_DIR ?= $(OUT_DIR)/objs
OBJ_FILE_MASK := $(OBJ_DIR)/%.o
OBJS := $(patsubst $(SRC_FILE_MASK),$(OBJ_FILE_MASK),$(SRCS))

DEP_DIR ?= $(OUT_DIR)/deps
DEP_FILE_MASK := $(DEP_DIR)/%.mk
DEPS := $(patsubst $(SRC_FILE_MASK),$(DEP_FILE_MASK),$(SRCS))

TARGET ?= $(OUT_DIR)/$(OUT_FILE)

.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	rm -rf $(OUT_DIR)

-include $(DEPS)

$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $@

$(OBJ_FILE_MASK): $(SRC_FILE_MASK)
	@mkdir -p $(patsubst $(OBJ_DIR)%,$(DEP_DIR)%, $(@D)) $(@D)
	$(CC) $(CFLAGS) -MMD -MP -MF $(patsubst $(OBJ_FILE_MASK),$(DEP_FILE_MASK),$@) -c $< -o $@

