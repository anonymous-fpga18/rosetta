# -------------------
# Check SDX environment
# -------------------
ifndef XILINX_SDX
$(error Environment variable XILINX_SDX is required and should point to SDx install area)
endif

# -------------------
# Tools used
# -------------------
CXX       = xcpp
XOCC      = xocc
SIMCONFIG = emconfigutil

# -------------------------------------
# Valid device list for SDAccel 2017.1
# -------------------------------------
VALID_DEVICES = xilinx:kcu1500:4ddr-xpr:4.0 \
                xilinx:adm-pcie-7v3:1ddr:3.0 \
                xilinx:xil-accel-rd-ku115:4ddr-xpr:4.0 \
                xilinx:adm-pcie-ku3:2ddr-xpr:4.0 \
                xilinx:zc706:linux-uart:1.0 \
                xilinx:adm-pcie-8k5:2ddr:4.0 \
                xilinx:adm-pcie-8k5:2ddr:3.3 \
                xilinx:adm-pcie-ku3:2ddr:3.3 \
                xilinx:adm-pcie-ku3:2ddr-xpr:3.3 \
                xilinx:xil-accel-rd-ku115:4ddr-xpr:3.3

# --------------------------------------
# Set Default OpenCL device and platform
# --------------------------------------
USR_PLATFORM = n
XDEVICE = xilinx:adm-pcie-7v3:1ddr:3.0
XPLATFORM = one_of_default_platforms

# --------------------------------
# Check OpenCL device and platform
# --------------------------------
ifeq ($(XPLATFORM), one_of_default_platforms)
  ifeq ($(filter $(XDEVICE),$(VALID_DEVICES)),)
    $(error "Device $(XDEVICE) is not supported. Supported devices are: $(VALID_DEVICES)")
  endif
else
  USR_PLATFORM=y
endif

# --------------------
# Check TARGET value
# --------------------
TARGET  = sw_emu
     ifeq ($(TARGET),sw_emu)
else ifeq ($(TARGET),hw_emu)
else ifeq ($(TARGET),hw)
else
$(error "TARGET does not support the $(TARGET) value. Supported values are: sw_emu, hw_emu, hw")
endif

# ----------------------
# Check kernel file type
# ----------------------
KERNEL_TYPE = ocl

ifeq ($(suffix $(KERNEL_SRC)),.cl)
  KERNEL_TYPE=ocl
else
  KERNEL_TYPE=c
endif

# -----------------
# Runtime Libraries
# -----------------
OPENCL_INC = $(XILINX_SDX)/runtime/include/1_2
OPENCL_LIB = $(XILINX_SDX)/runtime/lib/x86_64

# =========================================================
# HOST Sources and Host Executable files
# =========================================================
HOST_EXE        = host.exe
HARNESS_DIR     = ../../oclharness/src
HARNESS_SRC_CPP = ../../oclharness/src/CLKernel.cpp ../../oclharness/src/CLMemObj.cpp ../../oclharness/src/CLWorld.cpp
HARNESS_SRC_H   = ../../oclharness/src/CLKernel.h   ../../oclharness/src/CLMemObj.h   ../../oclharness/src/CLWorld.h

# =========================================================
# Kernel Sources and XCLBIN files
# =========================================================
XCLBIN        = kernels.$(TARGET).xclbin

# =========================================================
# Compilation
# =========================================================

.PHONY: all clean set_sim

all: $(HOST_EXE) $(XCLBIN)

# -----------------
# Host
# -----------------
$(HOST_EXE): $(HOST_SRC_CPP) $(HOST_SRC_H) $(HARNESS_SRC_CPP) $(HARNESS_SRC_H)
	$(CXX) -g -lxilinxopencl -I$(OPENCL_INC) $(HOST_INC) -L$(OPENCL_LIB) $(HOST_LIB) -I$(HARNESS_DIR) -o $@ $(HOST_SRC_CPP) $(HARNESS_SRC_CPP) 
 
# -----------------
# Kernels
# -----------------
ifeq ($(KERNEL_TYPE),ocl)
ifeq ($(USR_PLATFORM),n)
$(XCLBIN): $(KERNEL_SRC) $(KERNEL_H)
	$(XOCC) -s -t $(TARGET) -g --xdevice $(XDEVICE) $(KERNEL_ARGS) -o $@ $(KERNEL_SRC)
else
$(XCLBIN): $(KERNEL_SRC) $(KERNEL_H)
	$(XOCC) -s -t $(TARGET) -g --platform $(XPLATFORM) $(KERNEL_ARGS) -o $@ $(KERNEL_SRC)
endif
else
ifeq ($(USR_PLATFORM),n)
$(XCLBIN): $(KERNEL_SRC) $(KERNEL_H)
	$(XOCC) -s -t $(TARGET) --kernel $(KERNEL_NAME) -g --xdevice $(XDEVICE) $(KERNEL_ARGS) -o $@ $(KERNEL_SRC)
else
$(XCLBIN): $(KERNEL_SRC) $(KERNEL_H)
	$(XOCC) -s -t $(TARGET) --kernel $(KERNEL_NAME) -g --platform $(XPLATFORM) $(KERNEL_ARGS) -o $@ $(KERNEL_SRC)
endif
endif

# -----------------
# Simulation env
# -----------------
set_sim:
	$(SIMCONFIG) --xdevice $(XDEVICE)
		
clean:
	rm -rf _xocc_*
	rm -rf *.xclbin
	rm -rf *.exe
	rm -rf system_estimate.xtxt
	rm -rf .Xil
	rm -rf sdaccel_profile_summary.*
	rm -rf *.rpt
