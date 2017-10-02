/*===============================================================*/
/*                                                               */
/*                          kernel.h                             */
/*                                                               */
/*          Defines types and constants for OpenCL kernel        */
/*                                                               */
/*===============================================================*/

#include "ap_int.h"
#include "ap_fixed.h"

// dataset info
#define NUM_FEATURES      1024
#define NUM_SAMPLES       5000
#define NUM_TRAINING      4500
#define NUM_TESTING       500
#define STEP_SIZE         60000 //step size (eta)
#define NUM_EPOCHS        5
#define DATA_SET_SIZE     NUM_FEATURES * NUM_SAMPLES

// types
// datatype for feature vector
#define FTYPE_TWIDTH 32
#define FTYPE_IWIDTH 13
#define VFTYPE_WIDTH 64
typedef ap_fixed<FTYPE_TWIDTH,FTYPE_IWIDTH> FeatureType;
typedef ap_uint<VFTYPE_WIDTH>               VectorFeatureType;
const size_t F_VECTOR_SIZE = VFTYPE_WIDTH / FTYPE_TWIDTH;
// datatype for training data
#define DTYPE_TWIDTH 16
#define DTYPE_IWIDTH 4
#define VDTYPE_WIDTH 64
typedef ap_fixed<DTYPE_TWIDTH,DTYPE_IWIDTH>  DataType;
typedef ap_uint<VDTYPE_WIDTH>                VectorDataType;
const size_t D_VECTOR_SIZE = VDTYPE_WIDTH / DTYPE_TWIDTH;
// datatype for label
#define LTYPE_WIDTH 8
#define VLTYPE_WIDTH  32
typedef ap_uint<LTYPE_WIDTH>                 LabelType;
typedef ap_uint<VLTYPE_WIDTH>                VectorLabelType;
const size_t L_VECTOR_SIZE = VLTYPE_WIDTH / LTYPE_WIDTH;
// datatypes for the LUT
#define LUTOUT_TWIDTH     12
#define LUTOUT_IWIDTH     2
#define LUTIN_TWIDTH      12
#define LUTIN_IWIDTH      4
typedef ap_ufixed<32, 20> TmpFixed; 
typedef ap_uint<LUTIN_TWIDTH> IdxFixed; 
typedef ap_fixed<LUTIN_TWIDTH, LUTIN_IWIDTH> LutInFixed;
typedef ap_fixed<LUTOUT_TWIDTH, LUTOUT_IWIDTH> LutOutFixed;

// compute pipeline unroll factor
const int UNROLL_FACTOR = 32;

// top-level function declaration
#pragma SDS data mem_attribute(data:NON_CACHEABLE|PHYSICAL_CONTIGUOUS, label:CACHEABLE|PHYSICAL_CONTIGUOUS, theta:CACHEABLE|PHYSICAL_CONTIGUOUS)
#pragma SDS data data_mover(data:AXIDMA_SG, label:AXIFIFO, theta:AXIDMA_SG)
#pragma SDS data sys_port(data:AFI, label:AFI, theta:AFI)
#pragma SDS data access_pattern(data:SEQUENTIAL, label:SEQUENTIAL, theta: SEQUENTIAL) 
void SgdLR( VectorDataType    data[NUM_FEATURES * NUM_TRAINING / D_VECTOR_SIZE],
            VectorLabelType   label[NUM_TRAINING / L_VECTOR_SIZE],
            VectorFeatureType theta[NUM_FEATURES / F_VECTOR_SIZE]);

// software simulation switch
// #define SW_EMU
