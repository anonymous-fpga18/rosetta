/*===============================================================*/
/*                                                               */
/*                          typedefs.h                           */
/*                                                               */
/*          Typedefs for the host and hardware function          */
/*                                                               */
/*===============================================================*/

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <ap_int.h>

typedef ap_uint<1> bit1;
typedef ap_uint<2> bit2;
typedef ap_uint<8> bit8;
typedef ap_int<16>  s_bit8;   
typedef ap_int<9>  bit9;
typedef ap_uint<16> bit16;
typedef ap_uint<32> bit32;

// struct: 3D triangle
typedef struct
{
  bit8   x0;
  bit8   y0;
  bit8   z0;
  bit8   x1;
  bit8   y1;
  bit8   z1;
  bit8   x2;
  bit8   y2;
  bit8   z2;
} Triangle_3D;

// struct: 2D triangle
typedef struct
{
  bit8   x0;
  bit8   y0;
  bit8   x1;
  bit8   y1;
  bit8   x2;
  bit8   y2;
  bit8   z;
} Triangle_2D;


// struct: candidate pixels
typedef struct
{
  bit8   x;
  bit8   y;
  bit8   z;
  bit8   color;
} CandidatePixel;

// struct: colored pixel
typedef struct
{
  bit8   x;
  bit8   y;
  bit8   color;
} Pixel;


// resolution 256x256
#define MAX_X 256
#define MAX_Y 256

// number of values in frame buffer: 32 bits
#define NUM_FB MAX_X*MAX_Y/4
#define NUM_3D_TRI 3192

#endif
