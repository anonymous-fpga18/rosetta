# Optical Flow

## Description
This design computes the movement of each pixel in five continuous image frames. The hardware function constructs a streaming dataflow pipeline between different stages of the algorithm. The `dataset` directory contains our test input and the reference output flow file. The `imageLib` directory contains a small image library from Microsoft Research for image file I/O. 

## Usage
### SDAccel
To execute or simulate the design, do

`./host.exe -f <path_to_xclbin_file> -p <path_to_dataset> -o <path_to_output_file>`

### SDSoC
To run software model, do

`./oflow -p <path_to_dataset> -o <path_to_output_file>`

To run design on the board, do

`./oflow.elf -p <path_to_dataset> -o <path_to_output_file>`
