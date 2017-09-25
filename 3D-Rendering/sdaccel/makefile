#-------------------------
# Set kernel name
#-------------------------
KERNEL_NAME = rendering

#-------------------------
# Set host source and headers
#-------------------------
HOST_SRC_CPP = ./src/3d_rendering_host.cpp ./src/utils.cpp ./src/check_result.cpp
HOST_SRC_H   = ./src/utils.h ./src/check_result.h ./src/typedefs.h ./src/input_data.h
DATA         =

#-------------------------
# Set host code include paths
#-------------------------
HOST_INC = -I$(XILINX_SDX)/Vivado_HLS/include
HOST_LIB = -L$(XILINX_SDX)/Vivado_HLS/lib

#-------------------------
# Set kernel file
#-------------------------
KERNEL_SRC = ./src/rendering.cpp
KERNEL_H   = ./src/kernel.h

#-------------------------
# Set additional kernel arguments
#-------------------------
KERNEL_ARGS = --max_memory_ports all --report system

#-------------------------
# Leave the rest to harness
#-------------------------
include ../../oclharness/makefile/harness.mk

