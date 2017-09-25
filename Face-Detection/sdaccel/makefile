#-------------------------
# Set kernel name
#-------------------------
KERNEL_NAME = detectFaces

#-------------------------
# Set host source and headers
#-------------------------
HOST_SRC_CPP = ./src/face_detect.cpp ./src/utils.cpp ./src/check_result.cpp ./src/image.cpp
HOST_SRC_H   = ./src/utils.h ./src/check_result.h ./src/typedefs.h ./src/image0_320_240.h ./src/image.h
DATA         =

#-------------------------
# Set host code include paths
#-------------------------
HOST_INC = -I$(XILINX_SDX)/Vivado_HLS/include
HOST_LIB = -L$(XILINX_SDX)/Vivado_HLS/lib

#-------------------------
# Set kernel file
#-------------------------
KERNEL_SRC = ./src/haar.cpp
KERNEL_H   = ./src/haar.h

#-------------------------
# Set additional kernel arguments
#-------------------------
KERNEL_ARGS = --max_memory_ports all --report system

#-------------------------
# Leave the rest to harness
#-------------------------
include ../../oclharness/makefile/harness.mk

