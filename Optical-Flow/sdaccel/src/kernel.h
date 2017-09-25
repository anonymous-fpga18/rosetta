/*===============================================================*/
/*                                                               */
/*                          kernel.h                             */
/*                                                               */
/*        Defines types and constants for hardware kernel        */
/*                                                               */
/*===============================================================*/

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

// size of the image
#define MAX_HEIGHT 436
#define MAX_WIDTH 1024

#include "ap_int.h"

// basic typedefs
typedef float pixel_t;
typedef float vel_t;

typedef struct
{
  pixel_t x;
  pixel_t y;
  pixel_t z;
} gradient_t;

typedef struct
{
  pixel_t val[6];
} outer_t; 

typedef struct
{
  pixel_t val[6];
} tensor_t;

typedef struct
{
  pixel_t x;
  pixel_t y;
} velocity_t;

typedef ap_uint<64> frames_t;

#endif
