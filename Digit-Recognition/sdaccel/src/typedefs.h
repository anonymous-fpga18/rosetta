/*===============================================================*/
/*                                                               */
/*                          typedefs.h                           */
/*                                                               */
/*           Constant definitions and typedefs for host.         */
/*                                                               */
/*===============================================================*/

#include <string>

// target device
// change here to map to a different device
const std::string TARGET_DEVICE = "xilinx:adm-pcie-7v3:1ddr:3.0";

// dataset information
const int NUM_TRAINING  = 18000;
const int NUM_TEST      = 2000;
const int DIGIT_WIDTH   = 4;

// typedefs
typedef unsigned long long DigitType;
typedef unsigned char      LabelType;
