#-------------------------
# Set kernel name
#-------------------------
KERNEL_NAME = DigitRec

#-------------------------
# Set host source and headers
#-------------------------
HOST_SRC_CPP = ./src/digit_recognition.cpp ./src/utils.cpp ./src/check_result.cpp
HOST_SRC_H   = ./src/utils.h ./src/check_result.h ./src/typedefs.h ./src/testing_data.h ./src/training_data.h
DATA         = ../196data/*.dat

#-------------------------
# Set host code include paths
#-------------------------
HOST_INC = 
HOST_LIB = 

#-------------------------
# Set kernel file
#-------------------------
KERNEL_SRC = ./src/digitrec.cl
KERNEL_H   = ./src/kernel.h

#-------------------------
# Set additional kernel arguments
#-------------------------
KERNEL_ARGS = --max_memory_ports all --report system

#-------------------------
# Leave the rest to harness
#-------------------------
include ../../oclharness/makefile/harness.mk

