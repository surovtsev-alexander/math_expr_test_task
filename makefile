print-%  : ; @echo $* = $($*)

#region: build executable

SOURCE_DIR 	:=./sources
INCLUDE_DIR 	:=./include
BUILD_DIR 	:=./build

CC 		:=gcc

CFLAGS 		:=-c -Wall -I$(INCLUDE_DIR)
LINKING_C_FLAGS :=-Xlinker -Map=$(BUILD_DIR)/math_expr.map


SOURCES 	:=$(wildcard $(SOURCE_DIR)/*.c)
OBJECTS 	:=$(patsubst $(SOURCE_DIR)/%.c, 	$(BUILD_DIR)/%.o, 	$(SOURCES))

EXECUTABLE 	:=math_expr.exe

all: build_dir executable

build_dir:
	mkdir -p $(BUILD_DIR)

executable: $(BUILD_DIR)/$(EXECUTABLE)

$(BUILD_DIR)/$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LINKING_C_FLAGS)

$(OBJECTS): $(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(CC) -o $@ $< $(CFLAGS)

#endregion: build executable

start:
	$(BUILD_DIR)/$(EXECUTABLE)

clean:
	rm -rf $(BUILD_DIR)

