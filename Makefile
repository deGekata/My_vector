

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
uniq = $(if $1,$(firstword $1) $(call uniq,$(filter-out $(firstword $1),$1)))

# Applicaton options
APPLICATION = $(notdir $(CURDIR))# Application name is name of root holder
GXX_STANDARD = 14 # 11, 14, 17, 20
OUT_FILE_NAME = $(APPLICATION).exe

# Get project directory

# test:
# 	$(info    DIR is $(CWD))
# Сompiler options
CC = g++ 
CWD := $(subst /,\\, $(abspath $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))))
INCLUDE_PATHS := $(call uniq, $(sort $(dir $(call rwildcard,src, *.hpp *.cpp *.h))))

INCLUDE_PATHS := $(subst /,\\,$(dir $(INCLUDE_PATHS)))
INCLUDE_PATHS := $(patsubst %,-I$(CWD)\\\\% ,$(INCLUDE_PATHS)) 
LIBS_PATHS = 
CXXFLAGS = -Wall -Wextra -std=c++$(GXX_STANDARD) -fno-elide-constructors
CXXFLAGS += $(INCLUDE_PATHS) $(LIBS_PATHS)
LXXFLAGS = -fno-elide-constructors



# Debug or Release
# BUILD = Debug
BUILD = Release

# Folder name setting
SRC_DIR = src
BIN_DIR = bin

# Setting build parameters
ifeq ($(BUILD), Debug)
	CXXFLAGS += -O0 -g -fdiagnostics-color=always
	BUILD_PATH = build-debug
else
# CXXFLAGS += -O1 -g -fdiagnostics-color=always
	CXXFLAGS += -O0 -s -g -DNDEBUG 
	BUILD_PATH = build-debug
endif

# Search for source files
SRC_FULL_PATH = $(shell where /r .\ *.cpp)
EXLUDED = # Excluded files
SRC = $(filter-out $(EXLUDED),$(notdir $(SRC_FULL_PATH)))
OBJ = $(addprefix $(BIN_DIR)/, $(SRC:.cpp=.o))

# Include library
LIB_PATH = 
LIB_DEPEND = 
CXXFLAGS += $(patsubst %,-I%\\include,$(LIB_PATH))
CXXFLAGS += $(INCLUDE_PATHS)
LXXFLAGS += $(patsubst %,-L%\\lib,$(LIB_PATH)) $(addprefix -l, $(LIB_DEPEND))

# Сonfiguring file search paths
VPATH = echo $(subst \,/,$(dir $(SRC_FULL_PATH)))

# Build project
$(BUILD_PATH)/$(OUT_FILE_NAME): $(OBJ) Makefile
	@ $(CC) $(OBJ) -o $(BUILD_PATH)/$(OUT_FILE_NAME) $(LXXFLAGS)
	$(BUILD_PATH)/$(OUT_FILE_NAME)

# Dependency checking
include $(addprefix $(BIN_DIR)/, $(SRC:.cpp=.d))

# Compilation source
$(BIN_DIR)/%.o: %.cpp Makefile
	@ $(CC) $< -c -o $@ $(CXXFLAGS)

# Updating dependencies
$(BIN_DIR)/%.d: %.cpp Makefile
	@ $(CC) $<  $(CXXFLAGS) -MM -MT '$(BIN_DIR)/$*.o $(BIN_DIR)/$*.d' -MF $@ 

.PHONY: init clean
init:
	if not exist $(BIN_DIR) md $(BIN_DIR)
	if not exist $(BUILD_PATH) md $(BUILD_PATH)

clean:
	if exist $(CWD)\\$(BIN_DIR) rmdir /S /Q $(BIN_DIR)