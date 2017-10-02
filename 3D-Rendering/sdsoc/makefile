# Check SDX environment
ifndef XILINX_SDX
$(error Environment variable XILINX_SDX is required and should point to SDx install area)
endif

# source files
APPSOURCES = rendering.cpp rendering_test.cpp 

# executables
# sdsoc and pure software use different executables
EXECUTABLE = rendering.elf
SW_EXE     = 3d_rendering

# tools
SDSXX = sds++
CXX   = g++

# platform configuration
PLATFORM = zc706

# sds++ flags
SDSFLAGS = -sds-pf $(PLATFORM) -sds-hw rendering rendering.cpp -sds-end -clkid 3  \
	   -poll-mode 1 
SDSCFLAGS = -Wall -O3 -c
SDSCFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"
SDSLFLAGS = -O3

# sw compilation flags
CFLAGS = -I$(XILINX_SDX)/Vivado_HLS/include
LFLAGS = -L$(XILINX_SDX)/Vivado_HLS/lib

# objects
OBJECTS := $(APPSOURCES:.cpp=.o)
DEPS := $(OBJECTS:.o=.d)

.PHONY: all

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(SDSXX) $(SDSFLAGS) $(SDSLFLAGS) ${OBJECTS} -o $@ 

-include $(DEPS)

%.o: %.cpp
	$(SDSXX) $(SDSFLAGS) $(SDSCFLAGS) $< -o $@

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
