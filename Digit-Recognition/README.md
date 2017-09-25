# Digit Recognition

## Description
This design classifies hand-written digits using the K-Nearest-Neighbor (KNN) algorithm. The hardware accelerator compares the test instance with all training instances, and vote between the K nearest neighbors to get a final classification result. The images of training and test instances are binarized so we can exploit bit-level parallelism in the FPGA. The `196data` directory contains a binarized, downsampled subset of the MNIST database. 

## Usage
### SDAccel
To execute or simulate the design, do

`./host.exe -f <path_to_xclbin_file>`

### SDSoC
To run software model, do

`./digitrec`

To run design on the board, do

`./digitrec.elf`
