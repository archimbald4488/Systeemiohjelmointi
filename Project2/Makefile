CC = gcc
CFLAGS = -Wall -Werror -g

BUILD_DIR = build
TARGETS = $(BUILD_DIR)/my-cat $(BUILD_DIR)/my-grep $(BUILD_DIR)/my-zip $(BUILD_DIR)/my-unzip

all: $(BUILD_DIR) $(TARGETS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/my-cat: my-cat.c
	$(CC) $(CFLAGS) my-cat.c -o $(BUILD_DIR)/my-cat

$(BUILD_DIR)/my-grep: my-grep.c
	$(CC) $(CFLAGS) my-grep.c -o $(BUILD_DIR)/my-grep

$(BUILD_DIR)/my-zip: my-zip.c
	$(CC) $(CFLAGS) my-zip.c -o $(BUILD_DIR)/my-zip

$(BUILD_DIR)/my-unzip: my-unzip.c
	$(CC) $(CFLAGS) my-unzip.c -o $(BUILD_DIR)/my-unzip

clean:
	rm -rf $(BUILD_DIR)
