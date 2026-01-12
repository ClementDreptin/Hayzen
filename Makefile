# ================================================================
# Project configuration
# ================================================================

PROJECT_NAME := Hayzen
WINDOWS_SHIM := wine
CONFIG := Debug
CONSOLE_NAME := 192.168.1.14

BUILD_DIR := build
OUT_DIR := $(BUILD_DIR)/$(CONFIG)/bin
INT_DIR := $(BUILD_DIR)/$(CONFIG)/obj

TARGET := $(OUT_DIR)/$(PROJECT_NAME).xex


# ================================================================
# XexUtils dependency
# ================================================================

XEXUTILS_DIR := deps/XexUtils
XEXUTILS_TARGET_NAME := XexUtils
XEXUTILS_TARGET := $(XEXUTILS_DIR)/$(BUILD_DIR)/Release/bin/$(XEXUTILS_TARGET_NAME).lib
XEXUTILS_INC := $(XEXUTILS_DIR)/include


# ================================================================
# mINI dependency
# ================================================================

MINI_DIR := deps/mINI
MINI_TARGET_NAME := mINI
MINI_TARGET := $(MINI_DIR)/$(BUILD_DIR)/Release/bin/$(MINI_TARGET_NAME).lib
MINI_INC := $(MINI_DIR)


# ================================================================
# Source files
# ================================================================

ALL_SRCS := $(shell find src -name '*.cpp')

# Precompiled header sources
PCH_HEADER := pch.h
PCH_SRC := src/pch.cpp
PCH_OBJ := $(INT_DIR)/src/pch.obj
PCH_FILE := $(INT_DIR)/$(TARGET_NAME).pch

# Normal (non-PCH) sources
NORMAL_SRCS := $(filter-out $(PCH_SRC),$(ALL_SRCS))
NORMAL_OBJS := $(NORMAL_SRCS:%.cpp=$(INT_DIR)/%.obj)

# All objects that make up the final library
OBJS := $(NORMAL_OBJS) $(PCH_OBJ)


# ================================================================
# XDK Toolchain
# ================================================================

XDK_BIN_DIR := $(XEDK)/bin/win32
XDK_INC_DIR := "$(XEDK)/include/xbox"
XDK_LIB_DIR := "$(XEDK)/lib/xbox"

CXX := "$(XDK_BIN_DIR)/cl.exe"
LD := "$(XDK_BIN_DIR)/link.exe"
IMAGEXEX := "$(XDK_BIN_DIR)/imagexex.exe"
XBCP := "$(XDK_BIN_DIR)/xbcp.exe"
XBREBOOT := "$(XDK_BIN_DIR)/xbreboot.exe"


# ================================================================
# Compiler and linker flags
# ================================================================

INCLUDES := src $(XEXUTILS_INC) $(MINI_INC)
LIBS := xjson.lib

CXX_FLAGS := -c $(addprefix -I ,$(INCLUDES)) -Zi -nologo -W4 -MP -MT -D _XBOX -Gm- -EHsc -GS \
			 -fp:fast -fp:except- -Zc:wchar_t -Zc:forScope -GR- -openmp- -Fp"$(PCH_FILE)" \
			 -Fd"$(INT_DIR)/vc100.pdb" -TP -wd4481 -FI"$(XDK_INC_DIR)/xbox_intellisense_platform.h"

LD_FLAGS := -ERRORREPORT:QUEUE -NOLOGO $(LIBS) -MANIFESTUAC:"level='asInvoker' uiAccess='false'" \
			-DEBUG -PDB:"$(OUT_DIR)/$(PROJECT_NAME).pdb" -STACK:"262144","262144" -TLBID:1 \
			-RELEASE -IMPLIB:"$(OUT_DIR)/$(PROJECT_NAME).lib" -XEX:NO -ALIGN:128,4096 \
			-DLL -ENTRY:"_DllMainCRTStartup"

IMAGEXEX_FLAGS := -nologo -config:"config.xml"

ifeq ($(CONFIG),Debug)
	CXX_FLAGS += -WX- -Od -Gy- -GF-
else ifeq ($(CONFIG),Release)
	CXX_FLAGS += -WX -Ox -Oi -Os -D NDEBUG -Gy -GF
	LD_FLAGS += -OPT:REF
else
	$(error Unknown CONFIG=$(CONFIG))
endif


# ================================================================
# Top-level targets
# ================================================================

all: build copy

build: $(TARGET)

copy: $(TARGET)
	@echo "Copying $< to console..."
	@$(WINDOWS_SHIM) $(XBCP) -Y -X:$(CONSOLE_NAME) -T "$<" "hdd:\Plugins\$(PROJECT_NAME).xex"

clean:
	$(MAKE) -C $(XEXUTILS_DIR) $@
	$(MAKE) -C $(MINI_DIR) $@
	rm -rf $(BUILD_DIR)

.PHONY: all build copy clean


# ================================================================
# Final XEX
# ================================================================

$(TARGET): $(OUT_DIR)/$(PROJECT_NAME).exe
	@echo "Creating $(@)..."
	@mkdir -p $(@D)
	@$(WINDOWS_SHIM) $(IMAGEXEX) $(IMAGEXEX_FLAGS) -out:"$@" "$<"

$(OUT_DIR)/$(PROJECT_NAME).exe: $(OBJS) $(XEXUTILS_TARGET) $(MINI_TARGET)
	@echo "Linking $(@)..."
	@mkdir -p $(@D)
	@LIB=$(XDK_LIB_DIR) $(WINDOWS_SHIM) $(LD) $(LD_FLAGS) -OUT:"$@" $^

$(INT_DIR)/%.obj: %.cpp
	@mkdir -p $(@D)
	@INCLUDE=$(XDK_INC_DIR) $(WINDOWS_SHIM) $(CXX) $(CXX_FLAGS) -Fo"$@" $<


# ================================================================
# XexUtils build
# ================================================================

# We always link against the release build of XexUtils
# (see "About the debug configuration" in README.md for more info)
$(XEXUTILS_TARGET):
	$(MAKE) -C $(XEXUTILS_DIR) CONFIG=Release


# ================================================================
# mINI build
# ================================================================

# We always link against the release build of mINI
# (see "About the debug configuration" in README.md for more info)
$(MINI_TARGET):
	$(MAKE) -C $(MINI_DIR) CONFIG=Release
