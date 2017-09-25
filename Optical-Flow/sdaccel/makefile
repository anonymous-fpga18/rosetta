#-------------------------
# Set kernel name
#-------------------------
KERNEL_NAME = optical_flow

#-------------------------
# Set host source and headers
#-------------------------
HOST_SRC_CPP = ./src/optical_flow_host.cpp ./src/utils.cpp ./src/check_result.cpp ../imageLib/*.cpp
HOST_SRC_H   = ./src/utils.h ./src/check_result.h ./src/typedefs.h ../imageLib/*.h
DATA         =

#-------------------------
# Set host code include paths
#-------------------------
HOST_INC = -I$(XILINX_SDX)/Vivado_HLS/include -I../imageLib 
HOST_LIB = -L$(XILINX_SDX)/Vivado_HLS/lib 

#-------------------------
# Set kernel file
#-------------------------
KERNEL_SRC = ./src/optical_flow.cpp
KERNEL_H   = ./src/kernel.h

#-------------------------
# Set additional kernel arguments
#-------------------------
KERNEL_ARGS = --max_memory_ports all --report estimate

#-------------------------
# Leave the rest to harness
#-------------------------
include ../../oclharness/makefile/harness.mk

