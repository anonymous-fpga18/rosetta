/*===============================================================*/
/*                                                               */
/*                          typedefs.h                           */
/*                                                               */
/*              Constant definitions and typedefs.               */
/*                                                               */
/*===============================================================*/

#ifndef __DIGITREC__
#define __DIGITREC__

#include <ap_int.h>
#include <string>

// software simulation flag
//#define SW_EMU

// dataset information
const int NUM_TRAINING  = 18000;
const int NUM_TEST      = 2000;
const int DIGIT_WIDTH   = 4;
#define K_CONST           3
#define UNROLL_FACTOR     40

// typedefs
typedef ap_uint<256>  WholeDigitType;
typedef unsigned long long DigitType;
typedef unsigned char LabelType;

#endif
