# Face Detection

## Description
This design detects human faces from an image using the Viola Jones algorithm. The SDSoC design is directly adopted from the "Parallel and Pipelined" implementation by N. Srivastava et al. We rewrote the host code so the accelerator can also be executed on Xilinx OpenCL platforms. This implementation is in the `sdaccel` folder. 

## Usage
### SDAccel
To execute or simulate the design, do

`./host.exe -f <path_to_xclbin_file> -o <path_to_output_image>`

### SDSoC
Please refer to the original README file written by N. Srivastava et al. in the `sdsoc` directory. 
