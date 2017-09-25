/*===============================================================*/
/*                                                               */
/*                    digit_recognition.cpp                      */
/*                                                               */
/*   Main host function for the Digit Recognition application.   */
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
#include "training_data.h"
#include "testing_data.h"

using namespace rosetta;

int main(int argc, char ** argv) 
{
  printf("Digit Recognition Application\n");

  // parse command line arguments
  std::string kernelFile("");
  parse_sdaccel_command_line_args(argc, argv, kernelFile);

  // for this benchmark, data is already included in arrays:
  //   training_data: contains 18000 training samples, with 1800 samples for each digit class
  //   testing_data:  contains 2000 test samples
  //   expected:      contains labels for the test samples

  // create space for the result
  LabelType* result = new LabelType[NUM_TEST];

  /**************************OPENCL HOST CODE AREA***********************************/
    
  // create OpenCL world
  CLWorld digit_rec_world = CLWorld(TARGET_DEVICE, CL_DEVICE_TYPE_ACCELERATOR);

  // add the bitstream file
  digit_rec_world.addProgram(kernelFile);

  // create mem objects
  CLMemObj training_mem ( (void*)training_data,  sizeof(DigitType), NUM_TRAINING * DIGIT_WIDTH, CL_MEM_READ_ONLY);
  CLMemObj testing_mem  ( (void*)testing_data ,  sizeof(DigitType), NUM_TEST     * DIGIT_WIDTH, CL_MEM_READ_ONLY);
  CLMemObj result_mem   ( (void*)result       ,  sizeof(LabelType), NUM_TEST,                   CL_MEM_WRITE_ONLY);

  // add them to the world
  // added in sequence, each of them can be referenced by an index
  digit_rec_world.addMemObj(training_mem);
  digit_rec_world.addMemObj(testing_mem);
  digit_rec_world.addMemObj(result_mem);

  // create kernels
  CLKernel DigitRec(digit_rec_world.getContext(), digit_rec_world.getProgram(), "DigitRec", digit_rec_world.getDevice());

  // set work size
  int global_size[3] = {1, 1, 1};
  int local_size[3] = {1, 1, 1};
  DigitRec.set_global(global_size);
  DigitRec.set_local(local_size);

  // add them to the world
  digit_rec_world.addKernel(DigitRec);

  // set kernel arguments
  digit_rec_world.setMemKernelArg(0, 0, 0);
  digit_rec_world.setMemKernelArg(0, 1, 1);
  digit_rec_world.setMemKernelArg(0, 2, 2);

  // run!
  digit_rec_world.runKernels();

  // read the data back
  digit_rec_world.readMemObj(2);
  
  /**************************OPENCL HOST CODE AREA ENDS******************************/

  // check results
  printf("Checking results:\n");
  check_results( result, expected, NUM_TEST );
    
  // cleanup
  digit_rec_world.releaseWorld();

  delete []result;

  return EXIT_SUCCESS;

}
