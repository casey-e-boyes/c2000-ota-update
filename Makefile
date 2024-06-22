# ---------------------------------------------------------------------------- #
# Environment Variables & Tooling |------------------------------------------- #
# ---------------------------------------------------------------------------- #

# C2000 Compiler Executables and Includes
C2000_COMPILER_ROOT := $(HOME)/tooling/ti-cgt-c2000-22.6.1
C2000_COMPILER_EXES := $(C2000_COMPILER_ROOT)/bin
C2000_COMPILER_INCS := \
	-I$(C2000_COMPILER_ROOT)/include	\
	-I$(C2000_COMPILER_ROOT)/lib		\
	-I$(C2000_COMPILER_ROOT)/lib/src	\

# Add Compiler Executables to Shell Path and Set Default Shell to Bash
PATH := $(PATH):$(C2000_COMPILER_EXES)
SHELL = env PATH=$(PATH) /bin/bash

# C2000Ware Includes
C2000WARE_ROOT := $(HOME)/tooling/c2000ware
C2000WARE_INCS := \
	-I$(C2000WARE_ROOT)/f280049c/device_support/common/include	\
	-I$(C2000WARE_ROOT)/f280049c/device_support/headers/include	\
	-I$(C2000WARE_ROOT)/f280049c/driverlib						\
	-I$(C2000WARE_ROOT)/f280049c/driverlib/inc					\
	-I$(C2000WARE_ROOT)/libraries/vcu/include/common			\
	-I$(C2000WARE_ROOT)/libraries/vcu/include/vcu0				\

# ---------------------------------------------------------------------------- #
# Source Files, Include Directories, Libraries, and Linker Commands |--------- #
# ---------------------------------------------------------------------------- #

# Executable & Binary Name
TARGET := firmware

# Source Files Needed for Project
SOURCE_DIRECTORY := src
SOURCE_FILES := \
	$(C2000WARE_ROOT)/f280049c/device_support/common/source/f28004x_codestartbranch.asm		\
	$(C2000WARE_ROOT)/f280049c/device_support/headers/source/f28004x_globalvariabledefs.c	\
	$(SOURCE_DIRECTORY)/main.c		\
	$(SOURCE_DIRECTORY)/bsp/bsp.c	\

# Get List of Expected Object Files
SOURCE_FILENAMES := $(notdir $(basename $(SOURCE_FILES)))
OBJECT_DIRECTORY := build/obj
OBJECT_FILES := $(foreach src,$(SOURCE_FILENAMES),$(OBJECT_DIRECTORY)/$(src).o)

# Local Include Directories 
INCLUDE_DIRECTORIES := \
	-I$(SOURCE_DIRECTORY)			\
	-I$(SOURCE_DIRECTORY)/bsp		\
	-I$(SOURCE_DIRECTORY)/misc		\

# Libraries (Using Explicit Path)
LIBRARIES := \
	-l$(C2000_COMPILER_ROOT)/lib/rts2800_fpu32_eabi.lib					\
	-l$(C2000WARE_ROOT)/f280049c/driverlib/ccs/Debug/driverlib.lib		\
	-l$(C2000WARE_ROOT)/f280049c/driverlib/ccs/Debug/driverlib_eabi.lib	\
	-l$(C2000WARE_ROOT)/libraries/vcu/lib/c28x_vcu0_library_fpu32.lib	\

# Linkerscripts (Using Explicit Path)
LINKER_COMMAND_FILES := \
	-l$(C2000WARE_ROOT)/f280049c/device_support/common/cmd/28004x_generic_ram_lnk.cmd	\
	-l$(C2000WARE_ROOT)/f280049c/device_support/headers/cmd/f28004x_headers_nonbios.cmd	\

# ---------------------------------------------------------------------------- #
# C2000 C28X CPU Compiler Flags |--------------------------------------------- #
# ---------------------------------------------------------------------------- #

# Main Compiler Options & Hardware Options
C28XFLAGS += \
	--c11								\
	--abi=eabi							\
	--gen_data_subsections=on			\
	--gen_func_subsections=on			\
	--fp_mode=strict					\
	--silicon_version=28				\
	--float_support=fpu32				\
	--cla_support=cla2					\
	--tmu_support=tmu0					\
	--vcu_support=vcu0					\
	--silicon_errata_fpu1_workaround=on	\

# Compiler Defines
C28XFLAGS += \
	-D_DUAL_HEADERS						\

# Compiler Includes
C28XFLAGS += \
	$(C2000WARE_INCS)					\
	$(C2000_COMPILER_INCS)				\

# Build Options
C28XFLAGS += \
	--keep_asm							\
	--listing_extension=.list			\
	--obj_extension=.o					\
	-ft=build/temp						\
	-fs=build/asm						\
	-ff=build/list						\
	-fr=build/obj						\

# Debug Options
C28XFLAGS += \
	--symdebug:dwarf					\
	--symdebug:dwarf_version=4			\
	--quiet								\
	--display_error_number 				\
	--emit_warnings_as_errors			\
	--diag_wrap=on						\
	--diag_suppress=1					\

# Diagnostic Flag 1D - source file missing trailing newline

# ---------------------------------------------------------------------------- #
# C2000 Linker Flags |-------------------------------------------------------- #
# ---------------------------------------------------------------------------- #

C2000_LINKER_FLAGS := \
	--output_file=build/$(TARGET).elf	\
	--map_file=build/$(TARGET).map		\
	--stack_size=0x200					\
	--warn_sections						\
	--reread_libs						\
	--entry_point=code_start			\

# ---------------------------------------------------------------------------- #
# Make Recipies |------------------------------------------------------------- #
# ---------------------------------------------------------------------------- #

.DEFAULT_GOAL := build 



build:
	@mkdir -p build/{asm,list,obj,temp}
	@echo "[COMPILING]"
	@cl2000 $(C28XFLAGS) -c $(SOURCE_FILES)
	@echo "[LINKING]"
	@cl2000 $(C28XFLAGS) -z $(C2000_LINKER_FLAGS) $(OBJECT_FILES) $(LIBRARIES) $(LINKER_COMMAND_FILES)

clean:
	@rm -rf build



.PHONY: all build clean

# ---------------------------------------------------------------------------- #