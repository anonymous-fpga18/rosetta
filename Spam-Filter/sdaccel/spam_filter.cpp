/*===============================================================*/
/*                                                               */
/*                        spam_filter.cpp                        */
/*                                                               */
/*      Main host function for the Spam Filter application.      */
/*                                                               */
/*===============================================================*/

// standard C/C++ headers
#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <string>

// harness headers
#include "CLWorld.h"
#include "CLKernel.h"
#include "CLMemObj.h"

// other headers
#include "utils.h"
#include "typedefs.h"
#include "check_result.h"

using namespace rosetta;

int main(int argc, char *argv[]) 
{
  setbuf(stdout, NULL);

  printf("Spam Filter Application\n");

  // parse command line arguments
  std::string kernelFile("");
  std::string path_to_data("");
  parse_sdaccel_command_line_args(argc, argv, kernelFile, path_to_data);

  // allocate space
  // for software verification
  DataType*    data_points  = new DataType[DATA_SET_SIZE];
  LabelType*   labels       = new LabelType  [NUM_SAMPLES];
  FeatureType* param_vector = new FeatureType[NUM_FEATURES]; 
  // for compute
  VectorDataType* data_points_for_accel = new VectorDataType[NUM_TRAINING * NUM_FEATURES / D_VECTOR_SIZE];
  VectorLabelType* labels_for_accel = new VectorLabelType[NUM_TRAINING / L_VECTOR_SIZE];
  VectorFeatureType* param_for_accel = new VectorFeatureType[NUM_FEATURES / F_VECTOR_SIZE];

  // read in dataset
  std::string str_points_filepath = path_to_data + "/shuffledfeats.dat";
  std::string str_labels_filepath = path_to_data + "/shuffledlabels.dat";

  FILE* data_file;
  FILE* label_file;

  data_file = fopen(str_points_filepath.c_str(), "r");
  if (!data_file)
  {
    printf("Failed to open data file %s!\n", str_points_filepath.c_str());
    return EXIT_FAILURE;
  }
  for (int i = 0; i < DATA_SET_SIZE; i ++ )
  {
    float tmp;
    fscanf(data_file, "%f", &tmp);
    data_points[i] = tmp;
  }
  fclose(data_file);

  label_file = fopen(str_labels_filepath.c_str(), "r");
  if (!label_file)
  {
    printf("Failed to open label file %s!\n", str_labels_filepath.c_str());
    return EXIT_FAILURE;
  }
  for (int i = 0; i < NUM_SAMPLES; i ++ )
  {
    int tmp;
    fscanf(label_file, "%d", &tmp);
    labels[i] = tmp;
  }
  fclose(label_file);

  // reset parameter vector
  for (size_t i = 0; i < NUM_FEATURES; i++)
    param_vector[i] = 0;


  // reorganize data for the accelerator
  // in case anything weird happens
  // data points
  for (int i = 0; i < NUM_TRAINING * NUM_FEATURES / D_VECTOR_SIZE; i ++ )
    for (int j = 0; j < D_VECTOR_SIZE; j ++ )
      data_points_for_accel[i].range((j+1)*DTYPE_TWIDTH-1, j*DTYPE_TWIDTH) = data_points[i*D_VECTOR_SIZE+j].range(DTYPE_TWIDTH-1, 0);

  // labels
  for (int i = 0; i < NUM_TRAINING / L_VECTOR_SIZE; i ++ )
    for (int j = 0; j < L_VECTOR_SIZE; j ++ )
      labels_for_accel[i].range((j+1)*LTYPE_WIDTH-1, j*LTYPE_WIDTH) = labels[i*L_VECTOR_SIZE+j].range(LTYPE_WIDTH-1, 0);

  // parameter vector
  for (int i = 0; i < NUM_FEATURES / F_VECTOR_SIZE; i ++ )
    for (int j = 0; j < F_VECTOR_SIZE; j ++ )
      param_for_accel[i].range((j+1)*FTYPE_TWIDTH-1, j*FTYPE_TWIDTH) = param_vector[i*F_VECTOR_SIZE+j].range(FTYPE_TWIDTH-1, 0);


  /**************************OPENCL HOST CODE AREA***********************************/
    
  // create OpenCL world
  CLWorld spam_filter_world = CLWorld(TARGET_DEVICE, CL_DEVICE_TYPE_ACCELERATOR);

  // add the bitstream file
  spam_filter_world.addProgram(kernelFile);

  // create mem objects
  // use 4500 as training, 500 as test
  CLMemObj data_mem ( (void*)data_points_for_accel,  sizeof(VectorDataType),    NUM_TRAINING * NUM_FEATURES / D_VECTOR_SIZE, CL_MEM_READ_ONLY);
  CLMemObj label_mem( (void*)labels_for_accel,       sizeof(VectorLabelType),   NUM_TRAINING / L_VECTOR_SIZE,                CL_MEM_READ_ONLY);
  CLMemObj param_mem( (void*)param_for_accel, sizeof(VectorFeatureType), NUM_FEATURES / F_VECTOR_SIZE,                CL_MEM_READ_WRITE);

  // add them to the world
  // added in sequence, each of them can be referenced by an index
  spam_filter_world.addMemObj(data_mem);
  spam_filter_world.addMemObj(label_mem);
  spam_filter_world.addMemObj(param_mem);

  // create kernels
  CLKernel SgdLR      (spam_filter_world.getContext(), spam_filter_world.getProgram(), "SgdLR",       spam_filter_world.getDevice());

  // set work size
  int global_size[3] = {1, 1, 1};
  int local_size[3] = {1, 1, 1};
  SgdLR.set_global(global_size);
  SgdLR.set_local(local_size);

  // add them to the world
  // same with the index here
  spam_filter_world.addKernel(SgdLR);

  // set kernel arguments
  spam_filter_world.setMemKernelArg(0, 0, 0);
  spam_filter_world.setMemKernelArg(0, 1, 1);
  spam_filter_world.setMemKernelArg(0, 2, 2);

  // run!
  spam_filter_world.runKernels();

  // read the param vector back
  spam_filter_world.readMemObj(2);
  
  /**************************OPENCL HOST CODE AREA ENDS******************************/

  // reorganize the parameter vector
  for (int i = 0; i < NUM_FEATURES / F_VECTOR_SIZE; i ++ )
    for (int j = 0; j < F_VECTOR_SIZE; j ++ )
      param_vector[i*F_VECTOR_SIZE+j].range(FTYPE_TWIDTH-1, 0) = param_for_accel[i].range((j+1)*FTYPE_TWIDTH-1, j*FTYPE_TWIDTH);
 
  // check results
  printf("Checking results:\n");
  check_results( param_vector, data_points, labels );
    
  // cleanup
  spam_filter_world.releaseWorld();

  delete []data_points;
  delete []labels;
  delete []param_vector;
  delete []data_points_for_accel;
  delete []labels_for_accel;
  delete []param_for_accel;

  return EXIT_SUCCESS;
}
