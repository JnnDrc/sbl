CC := gcc
CFLAGS := -Wall -Wextra -Wno-type-limits -Wno-unused-parameter -I./src -O3

MYCFLAGS ?=

SRC_DIR := src

TARGET_SBL    := sbl
TARGET_SBLC   := sblc
TARGET_SBLI   := sbli
# TARGET_SBLR   := sblr
TARGETS       := $(TARGET_SBL) $(TARGET_SBLC) $(TARGET_SBLI)
TARGETS_MAIN  := $(addsuffix .c, $(TARGETS))


SRC := $(filter-out $(addprefix $(SRC_DIR)/, $(TARGETS_MAIN)),$(wildcard $(SRC_DIR)/*.c))
OBJ := $(patsubst %.c, %.o, $(SRC))

.PHONY: clean clean-obj all

all: $(TARGET_SBL) $(TARGET_SBLC) $(TARGET_SBLI) $(TARGET_SBLR)

$(OBJ): %.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(MYCFLAGS)

$(TARGET_SBL): %: $(SRC_DIR)/%.o $(OBJ)
	$(CC) -c $(SRC_DIR)/$@.c -o $(SRC_DIR)/$@.o $(CFLAGS) $(MYCFLAGS)
	$(CC) -o $@ $^

$(TARGET_SBLC): %: $(SRC_DIR)/%.o $(OBJ)
	$(CC) -c $(SRC_DIR)/$@.c -o $(SRC_DIR)/$@.o $(CFLAGS) $(MYCFLAGS)
	$(CC) -o $@ $^

$(TARGET_SBLI): %: $(SRC_DIR)/%.o $(OBJ)
	$(CC) -c $(SRC_DIR)/$@.c -o $(SRC_DIR)/$@.o $(CFLAGS) $(MYCFLAGS)
	$(CC) -o $@ $^

# $(TARGET_SBLR): %: $(SRC_DIR)/%.o $(OBJ)
# 	$(CC) -c $(SRC_DIR)/$@.c -o $(SRC_DIR)/$@.o $(CFLAGS) $(MYCFLAGS)
# 	$(CC) -o $@ $^

clean:
	rm -f $(OBJ) $(addprefix $(SRC_DIR)/, $(addsuffix .o, $(TARGETS))) $(TARGETS) 
clean-obj:
	rm -f $(OBJ) $(addprefix $(SRC_DIR)/, $(addsuffix .o, $(TARGETS)))
