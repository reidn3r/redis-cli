PROJECT_NAME = REDIS_CLI
BIN_DIR = out
BUILD_DIR = build
LIBS_DIR = libs

# Compiler
CC = gcc
CFLAGS = $(INC_FLAGS) -Wall -Wextra

# Sources
SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.c) \
      $(wildcard $(SRC_DIR)/*/*.c) \
      $(LIBS_DIR)/linenoise/linenoise.c

# Objects
OBJ = $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRC))

# Headers
INC_DIRS = include libs/linenoise
INC_FLAGS = $(addprefix -I,$(INC_DIRS))

# Linker
LDFLAGS = 

.PHONY: all clean run

all: $(BIN_DIR)/$(PROJECT_NAME)

$(BIN_DIR)/$(PROJECT_NAME): $(OBJ)
	@mkdir -p $(@D)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(BIN_DIR)/$(PROJECT_NAME)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)