# Check SDX environment
ifndef XILINX_SDX
$(error Environment variable XILINX_SDX is required and should point to SDx install area)
endif

# source files
APPSOURCES = optical_flow_host.cpp optical_flow.cpp utils.cpp check_result.cpp ../imageLib/Convert.cpp ../imageLib/Convolve.cpp ../imageLib/flowIO.cpp ../imageLib/Image.cpp ../imageLib/ImageIO.cpp ../imageLib/RefCntMem.cpp

# executables
# sdsoc and pure software use different executables
EXECUTABLE = oflow.elf
SW_EXE     = oflow

# tools
SDSXX = sds++
CXX   = g++

# platform configuration
PLATFORM = zc706

# sds++ flags
SDSFLAGS = -sds-pf $(PLATFORM) -sds-hw optical_flow optical_flow.cpp -sds-end -clkid 3  \
	   -poll-mode 1 
SDSFLAGS += -Wall -O3 -c -I../imageLib
SDSFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

# sw compilation flags
CFLAGS = -DSW_EMU -I$(XILINX_SDX)/Vivado_HLS/include -I../imageLib
LFLAGS = -L$(XILINX_SDX)/Vivado_HLS/lib

# objects
OBJECTS := $(APPSOURCES:.cpp=.o)
DEPS := $(OBJECTS:.o=.d)

.PHONY: all

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(SDSXX) $(SDSFLAGS) ${OBJECTS} -o $@ 

-include $(DEPS)

%.o: %.cpp
	$(SDSXX) $(SDSFLAGS) $< -o $@

sw:
	g++ -O1 $(CFLAGS) $(LFLAGS) $(APPSOURCES) -o $(SW_EXE)

clean:
	rm -rf $(EXECUTABLE)
	rm -rf $(EXECUTABLE).bit
	rm -rf $(OBJECTS) 
	rm -rf $(SW_EXE)
	rm -rf $(DEPS)
	rm -rf _sds
	rm -rf sd_card
	rm -rf .Xil
