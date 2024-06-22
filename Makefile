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

# ---------------------------------------------------------------------------- #



# ---------------------------------------------------------------------------- #
# Source Files, Include Directories, Libraries, and Linker Commands |--------- #
# ---------------------------------------------------------------------------- #

# Source Files Needed for Project
SOURCE_DIRECTORY := src
SOURCE_FILES := \
	$(C2000WARE_ROOT)/f280049c/device_support/common/source/f28004x_codestartbranch.asm		\
	$(C2000WARE_ROOT)/f280049c/device_support/headers/source/f28004x_globalvariabledefs.c	\
	$(SOURCE_DIRECTORY)/main.c	\

# Get List of Expected Object Files
SOURCE_FILENAMES := $(notdir $(basename $(SOURCE_FILES)))
OBJECT_DIRECTORY := build/obj
OBJECT_FILES := $(foreach src,$(SOURCE_FILENAMES),$(OBJECT_DIRECTORY)/$(src).o)

# Local Include Directories 
INCLUDE_DIRECTORIES := \
	-I$(SOURCE_DIRECTORY)	\

# Libraries (Explicit Path)
LIBRARIES := \
	-L$(C2000_COMPILER_ROOT)/lib/rts2800_fpu32_eabi.lib					\
	-L$(C2000WARE_ROOT)/f280049c/driverlib/ccs/Debug/driverlib.lib		\
	-L$(C2000WARE_ROOT)/f280049c/driverlib/ccs/Debug/driverlib_eabi.lib	\

# ---------------------------------------------------------------------------- #



# ---------------------------------------------------------------------------- #
# C2000 C28X CPU Compiler & Linker Flags |------------------------------------ #
# ---------------------------------------------------------------------------- #





# ---------------------------------------------------------------------------- #