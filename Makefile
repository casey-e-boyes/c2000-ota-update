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

# Add compiler executables to shell path and change/set defaul shell to bash
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


all:
	cl2000