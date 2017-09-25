/*===============================================================*/
/*                                                               */
/*                          kernel.h                             */
/*                                                               */
/*        Defines types and constants for host function          */
/*                                                               */
/*===============================================================*/

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#define MAX_HEIGHT 436
#define MAX_WIDTH 1024

#include "ap_int.h"

// basic typedefs
typedef float pixel_t;

typedef struct{
    pixel_t x;
    pixel_t y;
    pixel_t z;
}gradient_t;

typedef struct{
    pixel_t val[6];
}outer_t; 

typedef struct{
    pixel_t val[6];
}tensor_t;

typedef struct{
    pixel_t x;
    pixel_t y;
}velocity_t;

typedef ap_uint<64> frames_t;

// change the target device here
const std::string TARGET_DEVICE = "xilinx:adm-pcie-7v3:1ddr:3.0";

#endif
