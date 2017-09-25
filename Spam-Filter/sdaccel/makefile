#-------------------------
# Set kernel name
#-------------------------
KERNEL_NAME = SgdLR

#-------------------------
# Set host source and headers
#-------------------------
HOST_SRC_CPP = ./src/spam_filter.cpp ./src/utils.cpp ./src/check_result.cpp
HOST_SRC_H   = ./src/utils.h ./src/check_result.h ./src/typedefs.h
DATA         =

#-------------------------
# Set host code include paths
#-------------------------
HOST_INC = -I$(XILINX_SDX)/Vivado_HLS/include
HOST_LIB = -L$(XILINX_SDX)/Vivado_HLS/lib

#-------------------------
# Set kernel file
#-------------------------
KERNEL_SRC = ./src/sgd.cpp
KERNEL_H   = ./src/kernel.h ./src/lut.h

#-------------------------
# Set additional kernel arguments
#-------------------------
KERNEL_ARGS = --max_memory_ports all --report estimate

#-------------------------
# Leave the rest to harness
#-------------------------
include ../../oclharness/makefile/harness.mk

