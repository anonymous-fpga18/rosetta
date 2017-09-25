# Rosetta: A Realistic Benchmark Suite for HLS
Rosetta is a realistic benchmark suite for high-level synthesis. It contains six fully-developed applications from machine learning and image processing fields. Currently Rosetta supports Xilinx SDAccel 2017.1 and SDSoC 2017.1 development environments. We will continue to include more applications and optimize existing benchmarks. 

## Applications
1. 3D rendering;
2. Digit recognition;
3. Spam filtering;
4. Optical flow;
5. Binarized neural network, adopted from [https://github.com/cornell-zhang/bnn-fpga];
6. Face detection, adopted from [https://github.com/cornell-zhang/facedetect-fpga].

## General Structure
The `oclharness` directory contains the wrapper code for OpenCL APIs, as well as the main makefile for Xilinx SDAccel. For each benchmark, the `sdaccel` directory contains the host and kernel code for Xilinx SDAccel, while the `sdsoc` directory contains the code for Xilinx SDSoC. 

We also include original SDSoC implementations of BNN and face detection in this repo. To compile those designs please refer to the README files in those directories. 

## Usage
### SDAccel benchmark compilation steps:
1. Go into any benchmark folder.
2. Go into the `sdaccel` directory.
3. To compile for software emulation and get a quick latency estimate, do `make TARGET=sw_emu`. The report `system_estimate.xtxt` shows latency and resource estimate after high-level synthesis. If only a software model is needed, comment out `--report estimate` from the local makefile. Compilation time will significantly decrease. 
4. To compile for hardware emulation, do `make TARGET=hw_emu`.
5. To compile for bitstream and actually execute on the board, do `make TARGET=hw`.
6. Target platform can be specified with the `XDEVICE` variable. Default is Alpha Data 7v3 board. For example, to target the Alpha Data KU3 board and generate bitstream, do `make TARGET=hw XDEVICE=xilinx:adm-pcie-ku3:2ddr-xpr:4.0`. Also remember to change the target device string in header files. 
7. SDAccel also supports using custom platforms which are not integrated yet. To use a custom platform, specify its path with the `XPLATFORM` variable. For example, to generate bitstream for a custom platform, do `make TARGET=hw XPLATFORM=<path_to_custom_platform_xfpm_file>`. 
8. To run simulations, please follow the instructions on SDAccel user guide. 

### SDSoC benchmark compilation steps:
1. Go into any benchmark folder. 
2. Go into the `sdsoc` directory. 
3. To compile for software model, do `make sw`. 
4. To actually generate bitstream, do `make`. 
5. The target platform is now hard-coded in the makefiles. All benchmarks currently target the ZC706 platform.  
