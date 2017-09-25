/*===============================================================*/
/*                                                               */
/*                          typedefs.h                           */
/*                                                               */
/*              Constant definitions and typedefs.               */
/*                                                               */
/*===============================================================*/

#include <string>
#include "ap_int.h"
#include "ap_fixed.h"

// target device
// change here to map to a different device
const std::string TARGET_DEVICE = "xilinx:adm-pcie-7v3:1ddr:3.0";

// dataset information
const int NUM_SAMPLES   = 5000;
const int NUM_TRAINING  = 4500;
const int NUM_TEST      = 500;
const int NUM_FEATURES  = 1024;
const int DATA_SET_SIZE = NUM_SAMPLES * NUM_FEATURES;

// typedefs
#define FTYPE_TWIDTH 32
#define FTYPE_IWIDTH 13
#define VFTYPE_WIDTH  512
typedef ap_fixed<FTYPE_TWIDTH,FTYPE_IWIDTH> FeatureType;
typedef ap_uint<VFTYPE_WIDTH>               VectorFeatureType;
const size_t F_VECTOR_SIZE = VFTYPE_WIDTH / FTYPE_TWIDTH;
// datatype for training data
#define DTYPE_TWIDTH 16
#define DTYPE_IWIDTH 4
#define VDTYPE_WIDTH  512
typedef ap_fixed<DTYPE_TWIDTH,DTYPE_IWIDTH>  DataType;
typedef ap_uint<VDTYPE_WIDTH>                VectorDataType;
const size_t D_VECTOR_SIZE = VDTYPE_WIDTH / DTYPE_TWIDTH;
// datatype for label
#define LTYPE_WIDTH 8
#define VLTYPE_WIDTH  32
typedef ap_uint<LTYPE_WIDTH>                 LabelType;
typedef ap_uint<VLTYPE_WIDTH>                VectorLabelType;
const size_t L_VECTOR_SIZE = VLTYPE_WIDTH / LTYPE_WIDTH;
