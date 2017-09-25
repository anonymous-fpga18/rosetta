/*===============================================================*/
/*                                                               */
/*                       face_detect.cpp                         */
/*                                                               */
/*     Main host function for the Face Detection application.    */
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

// data
#include "image0_320_240.h"

using namespace rosetta;

int main(int argc, char ** argv) 
{
  printf("Face Detection Application\n");

  // parse command line arguments
  std::string kernelFile("");
  std::string outFile("");
  parse_sdaccel_command_line_args(argc, argv, kernelFile, outFile);

  // create space for input and output

  // for this benchmark, input data is included in array Data
  // these are outputs
  int result_x[RESULT_SIZE];
  int result_y[RESULT_SIZE];
  int result_w[RESULT_SIZE];
  int result_h[RESULT_SIZE];
  int res_size = 0;

  /**************************OPENCL HOST CODE AREA***********************************/
    
  // create OpenCL world
  CLWorld facedetect_world = CLWorld(TARGET_DEVICE, CL_DEVICE_TYPE_ACCELERATOR);

  // add the bitstream file
  facedetect_world.addProgram(kernelFile);

  // create mem objects
  CLMemObj input_mem   ( (void*)Data,        sizeof(unsigned char), IMAGE_HEIGHT * IMAGE_WIDTH, CL_MEM_READ_ONLY);
  CLMemObj res_x_mem   ( (void*)result_x,    sizeof(int),           RESULT_SIZE,                CL_MEM_WRITE_ONLY);
  CLMemObj res_y_mem   ( (void*)result_y,    sizeof(int),           RESULT_SIZE,                CL_MEM_WRITE_ONLY);
  CLMemObj res_w_mem   ( (void*)result_w,    sizeof(int),           RESULT_SIZE,                CL_MEM_WRITE_ONLY);
  CLMemObj res_h_mem   ( (void*)result_h,    sizeof(int),           RESULT_SIZE,                CL_MEM_WRITE_ONLY);
  CLMemObj res_size_mem( (void*)(&res_size), sizeof(int),           1,                          CL_MEM_WRITE_ONLY);

  // add them to the world
  // added in sequence, each of them can be referenced by an index
  facedetect_world.addMemObj(input_mem);
  facedetect_world.addMemObj(res_x_mem);
  facedetect_world.addMemObj(res_y_mem);
  facedetect_world.addMemObj(res_w_mem);
  facedetect_world.addMemObj(res_h_mem);
  facedetect_world.addMemObj(res_size_mem);

  // create kernels
  // this kernel is written in C++
  CLKernel FaceDetect(facedetect_world.getContext(), facedetect_world.getProgram(), "detectFaces", facedetect_world.getDevice());

  // set work size
  int global_size[3] = {1, 1, 1};
  int local_size[3] = {1, 1, 1};
  FaceDetect.set_global(global_size);
  FaceDetect.set_local(local_size);

  // add them to the world
  facedetect_world.addKernel(FaceDetect);

  // set kernel arguments
  facedetect_world.setMemKernelArg(0, 0, 0);
  facedetect_world.setMemKernelArg(0, 1, 1);
  facedetect_world.setMemKernelArg(0, 2, 2);
  facedetect_world.setMemKernelArg(0, 3, 3);
  facedetect_world.setMemKernelArg(0, 4, 4);
  facedetect_world.setMemKernelArg(0, 5, 5);

  // run!
  facedetect_world.runKernels();

  // read the data back
  facedetect_world.readMemObj(1);
  facedetect_world.readMemObj(2);
  facedetect_world.readMemObj(3);
  facedetect_world.readMemObj(4);
  facedetect_world.readMemObj(5);
  
  /**************************OPENCL HOST CODE AREA ENDS******************************/

  // check results
  printf("Checking results:\n");
  check_results(res_size, result_x, result_y, result_w, result_h, Data, outFile);
    
  // cleanup
  facedetect_world.releaseWorld();

  return EXIT_SUCCESS;

}
