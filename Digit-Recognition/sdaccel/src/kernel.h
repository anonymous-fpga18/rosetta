/*===============================================================*/
/*                                                               */
/*                          kernel.h                             */
/*                                                               */
/*          Defines types and constants for OpenCL kernel        */
/*                                                               */
/*===============================================================*/

// dataset info
#define NUM_TRAINING      18000
#define NUM_TESTING       2000
#define K_CONST           5
#define UNROLL_FACTOR     40

// types
typedef ulong4        WholeDigitType;
typedef unsigned char LabelType;

