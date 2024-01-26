SRC_DIR := src
BIN_DIR := bin

EXEC_NAME := bv

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%.o,$(SRCS))

CC := gcc
CFLAGS := -Wall -Wextra
LDFLAGS := -lcurl

LOGGER = @echo "[$(notdir $<)]"

all: $(BIN_DIR)/$(EXEC_NAME)

$(BIN_DIR)/$(EXEC_NAME): $(OBJS)
	$(LOGGER) Linking...
	@mkdir -p $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $@ $^
	@echo "done."

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(LOGGER) Compiling...
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<
	@echo "done."

start:
	@echo "$(EXEC_NAME) Build"

clean:
	rm -rf $(BIN_DIR)

.PHONY: all start clean install

install: $(BIN_DIR)/$(EXEC_NAME)
	@sudo cp -f $< /usr/local/bin/$(EXEC_NAME)
	@echo "Installed $(EXEC_NAME) to /usr/local/bin/"

uninstall:
	@sudo rm -f /usr/local/bin/$(EXEC_NAME)
	@echo "Uninstalled $(EXEC_NAME) from /usr/local/bin/"
