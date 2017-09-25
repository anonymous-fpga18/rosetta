/*===============================================================*/
/*                                                               */
/*                          typedefs.h                           */
/*                                                               */
/*             Typedefs for the hardware function                */
/*                                                               */
/*===============================================================*/

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include "ap_int.h"

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
}outer_t;

typedef struct
{
  pixel_t val[6];
}tensor_t;

typedef struct
{
  pixel_t x;
  pixel_t y;
} velocity_t;

typedef ap_uint<64> frames_t;

#endif
