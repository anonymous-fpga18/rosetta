/*===============================================================*/
/*                                                               */
/*                       3d_rendering.cpp                        */
/*                                                               */
/*      Main host function for the 3D Rendering application.     */
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
#include "input_data.h"

using namespace rosetta;

int main(int argc, char ** argv) 
{
  printf("3D Rendering Application\n");

  // parse command line arguments
  std::string kernelFile("");
  parse_sdaccel_command_line_args(argc, argv, kernelFile);

  // create space for input and output
  bit32* input  = new bit32[3 * NUM_3D_TRI];
  bit32* output = new bit32[NUM_FB];

  // for this benchmark, data is included in array triangle_3ds
  // however, we need to reorganize it for better performance
  for ( int i = 0; i < NUM_3D_TRI; i ++)
  {
    input[3*i](7,0)     = triangle_3ds[i].x0;
    input[3*i](15,8)    = triangle_3ds[i].y0;
    input[3*i](23,16)   = triangle_3ds[i].z0;
    input[3*i](31,24)   = triangle_3ds[i].x1;
    input[3*i+1](7,0)   = triangle_3ds[i].y1;
    input[3*i+1](15,8)  = triangle_3ds[i].z1;
    input[3*i+1](23,16) = triangle_3ds[i].x2;
    input[3*i+1](31,24) = triangle_3ds[i].y2;
    input[3*i+2](7,0)   = triangle_3ds[i].z2;
    input[3*i+2](31,8)  = 0;
  }


  /**************************OPENCL HOST CODE AREA***********************************/
    
  // create OpenCL world
  CLWorld rendering_world = CLWorld(TARGET_DEVICE, CL_DEVICE_TYPE_ACCELERATOR);

  // add the bitstream file
  rendering_world.addProgram(kernelFile);

  // create mem objects
  CLMemObj input_mem ( (void*)input,  sizeof(bit32), 3 * NUM_3D_TRI, CL_MEM_READ_ONLY);
  CLMemObj output_mem( (void*)output, sizeof(bit32), NUM_FB,         CL_MEM_WRITE_ONLY);

  // add them to the world
  // added in sequence, each of them can be referenced by an index
  rendering_world.addMemObj(input_mem);
  rendering_world.addMemObj(output_mem);

  // create kernels
  // this kernel is written in C++
  CLKernel Rendering(rendering_world.getContext(), rendering_world.getProgram(), "rendering", rendering_world.getDevice());

  // set work size
  int global_size[3] = {1, 1, 1};
  int local_size[3] = {1, 1, 1};
  Rendering.set_global(global_size);
  Rendering.set_local(local_size);

  // add them to the world
  rendering_world.addKernel(Rendering);

  // set kernel arguments
  rendering_world.setMemKernelArg(0, 0, 0);
  rendering_world.setMemKernelArg(0, 1, 1);

  // run!
  rendering_world.runKernels();

  // read the data back
  rendering_world.readMemObj(1);
  
  /**************************OPENCL HOST CODE AREA ENDS******************************/

  // check results
  printf("Checking results:\n");
  check_results(output);
    
  // cleanup
  rendering_world.releaseWorld();

  delete []input;
  delete []output;

  return EXIT_SUCCESS;

}
