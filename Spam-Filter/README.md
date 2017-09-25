# Spam Filtering

## Description
This design trains a logistic regression model to classify emails as either "spam" or "ham". We use stochastic gradient descent (SGD) in our accelerator. The `dataset` directory contains our test dataset with 5000 emails. 

## Usage
### SDAccel
To execute or simulate the design, do

`./host.exe -f <path_to_xclbin_file> -p <path_to_dataset>`

### SDSoC
To run software model, do

`./spam_filter -p <path_to_dataset>`

To run design on the board, do

`./spam_filter.elf -p <path_to_dataset>`
