//========================================================================
// Rendering Header File
//========================================================================
// @brief: This header file defines the interface for the core function

#ifndef RENDERING_H
#define RENDERING_H

#include "typedefs.h"
/*
  rendering function
*/
#pragma SDS data access_pattern(input:SEQUENTIAL, output:SEQUENTIAL)
void rendering(bit32 input[3*NUM_3D_TRI], bit32 output[NUM_FB]);

#endif

