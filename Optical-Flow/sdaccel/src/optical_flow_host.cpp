/*===============================================================*/
/*                                                               */
/*                    optical_flow_host.cpp                      */
/*                                                               */
/*      Main host function for the Optical Flow application.     */
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

// here we use an image library to handle file IO
#include "imageLib.h"

// other headers
#include "utils.h"
#include "typedefs.h"
#include "check_result.h"

using namespace rosetta;

int main(int argc, char ** argv) 
{
  setbuf(stdout, NULL);
  printf("Optical Flow Application\n");

  // parse command line arguments
  std::string kernelFile("");
  std::string dataPath("");
  std::string outFile("");
  parse_sdaccel_command_line_args(argc, argv, kernelFile, dataPath, outFile);

  // create actual file names according to the datapath
  std::string frame_files[5];
  std::string reference_file;
  frame_files[0] = dataPath + "/frame1.ppm";
  frame_files[1] = dataPath + "/frame2.ppm";
  frame_files[2] = dataPath + "/frame3.ppm";
  frame_files[3] = dataPath + "/frame4.ppm";
  frame_files[4] = dataPath + "/frame5.ppm";

  reference_file = dataPath + "/ref.flo";

  // arrays for compute
  // inputs
  frames_t*  frames = new frames_t[MAX_HEIGHT * MAX_WIDTH];

  // output
  velocity_t* outputs = new velocity_t[MAX_HEIGHT * MAX_WIDTH];

  // read in images and convert to grayscale
  printf("Reading files ... \n");

  CByteImage imgs[5];
  for (int i = 0; i < 5; i++) 
  {
    CByteImage tmpImg;
    ReadImage(tmpImg, frame_files[i].c_str());
    imgs[i] = ConvertToGray(tmpImg);
  }
  
  // normalize and assign the values
  for (int i = 0; i < MAX_HEIGHT; i++) 
  {
    for (int j = 0; j < MAX_WIDTH; j++) 
    {
      frames[i*MAX_WIDTH+j](7 ,  0) = imgs[0].Pixel(j, i, 0);
      frames[i*MAX_WIDTH+j](15,  8) = imgs[1].Pixel(j, i, 0);
      frames[i*MAX_WIDTH+j](23, 16) = imgs[2].Pixel(j, i, 0);
      frames[i*MAX_WIDTH+j](31, 24) = imgs[3].Pixel(j, i, 0);
      frames[i*MAX_WIDTH+j](39, 32) = imgs[4].Pixel(j, i, 0);
      frames[i*MAX_WIDTH+j](63, 40) = 0;  
    }
  }

  // read in reference flow file
  printf("Reading flow... \n");

  CFloatImage refFlow;
  ReadFlowFile(refFlow, reference_file.c_str());

  /**************************OPENCL HOST CODE AREA***********************************/
    
  // create OpenCL world
  CLWorld oflow_world = CLWorld(TARGET_DEVICE, CL_DEVICE_TYPE_ACCELERATOR);

  // add the bitstream file
  oflow_world.addProgram(kernelFile);

  // create mem objects
  CLMemObj frames_mem ( (void*)frames,  sizeof(frames_t),   MAX_HEIGHT * MAX_WIDTH, CL_MEM_READ_ONLY, 0);
  CLMemObj outputs_mem( (void*)outputs, sizeof(velocity_t), MAX_HEIGHT * MAX_WIDTH, CL_MEM_WRITE_ONLY, 0);

  // add them to the world
  // added in sequence, each of them can be referenced by an index
  oflow_world.addMemObj(frames_mem);
  oflow_world.addMemObj(outputs_mem);

  // create kernels
  // this kernel is written in C++
  CLKernel OpticalFlow(oflow_world.getContext(), oflow_world.getProgram(), "optical_flow", oflow_world.getDevice());

  // set work size
  int global_size[3] = {1, 1, 1};
  int local_size[3] = {1, 1, 1};
  OpticalFlow.set_global(global_size);
  OpticalFlow.set_local(local_size);

  // add them to the world
  oflow_world.addKernel(OpticalFlow);

  // set kernel arguments
  oflow_world.setMemKernelArg(0, 0, 0);
  oflow_world.setMemKernelArg(0, 1, 1);
 
  // run!
  oflow_world.runKernels();

  // read the data back
  oflow_world.readMemObj(1);

  /**************************OPENCL HOST CODE AREA ENDS******************************/

  // check results
  printf("Checking results:\n");

  check_results(outputs, refFlow, outFile);


  // cleanup
  oflow_world.releaseWorld();

  delete []frames;
  delete []outputs; 

  return EXIT_SUCCESS;

}
