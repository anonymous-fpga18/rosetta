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

// timer
#include <time.h>
#include <sys/time.h>

// other headers
#include "typedefs.h"
#include "check_result.h"
#include "digitrec.h"

#ifndef SW_EMU
  #include "sds_lib.h"
#endif

// data
#include "training_data.h"
#include "testing_data.h"

int main(int argc, char ** argv) 
{
  printf("Digit Recognition Application\n");

  // for this benchmark, data is already included in arrays:
  //   training_data: contains 18000 training samples, with 1800 samples for each digit class
  //   testing_data:  contains 2000 test samples
  //   expected:      contains labels for the test samples
  
  // reorganize data into our desired format
  #ifndef SW_EMU
    WholeDigitType* training_in0 = (WholeDigitType*)sds_alloc(sizeof(WholeDigitType) * NUM_TRAINING / 2);
    WholeDigitType* training_in1 = (WholeDigitType*)sds_alloc(sizeof(WholeDigitType) * NUM_TRAINING / 2);
    WholeDigitType* test_in      = (WholeDigitType*)sds_alloc(sizeof(WholeDigitType) * NUM_TEST);
  #else
    WholeDigitType* training_in0 = (WholeDigitType*)malloc(sizeof(WholeDigitType) * NUM_TRAINING / 2);
    WholeDigitType* training_in1 = (WholeDigitType*)malloc(sizeof(WholeDigitType) * NUM_TRAINING / 2);
    WholeDigitType* test_in      = (WholeDigitType*)malloc(sizeof(WholeDigitType) * NUM_TEST);
  #endif

  for (int i = 0; i < NUM_TRAINING / 2; i ++ )
  {
    training_in0[i].range(63 , 0  ) = training_data[i*DIGIT_WIDTH+0];
    training_in0[i].range(127, 64 ) = training_data[i*DIGIT_WIDTH+1];
    training_in0[i].range(191, 128) = training_data[i*DIGIT_WIDTH+2];
    training_in0[i].range(255, 192) = training_data[i*DIGIT_WIDTH+3];
  }
  for (int i = 0; i < NUM_TRAINING / 2; i ++ )
  {
    training_in1[i].range(63 , 0  ) = training_data[(NUM_TRAINING / 2 + i)*DIGIT_WIDTH+0];
    training_in1[i].range(127, 64 ) = training_data[(NUM_TRAINING / 2 + i)*DIGIT_WIDTH+1];
    training_in1[i].range(191, 128) = training_data[(NUM_TRAINING / 2 + i)*DIGIT_WIDTH+2];
    training_in1[i].range(255, 192) = training_data[(NUM_TRAINING / 2 + i)*DIGIT_WIDTH+3];
  }

  for (int i = 0; i < NUM_TEST; i ++ )
  {
    test_in[i].range(63 , 0  ) = testing_data[i*DIGIT_WIDTH+0];
    test_in[i].range(127, 64 ) = testing_data[i*DIGIT_WIDTH+1];
    test_in[i].range(191, 128) = testing_data[i*DIGIT_WIDTH+2];
    test_in[i].range(255, 192) = testing_data[i*DIGIT_WIDTH+3];
  }

  // create space for the result
  #ifndef SW_EMU
    LabelType* result = (LabelType*)sds_alloc(sizeof(LabelType) * NUM_TEST);
  #else
    LabelType* result = (LabelType*)malloc(sizeof(LabelType) * NUM_TEST);
  #endif
  
  // run the hardware function
  struct timeval start, end;
  // first call: transfer data
  DigitRec(training_in0, test_in, result, 0);
  // second call: execute
  gettimeofday(&start, NULL);
  DigitRec(training_in1, test_in, result, 1);
  gettimeofday(&end, NULL);

  // check results
  printf("Checking results:\n");
  check_results( result, expected, NUM_TEST );

  printf("Elapsed time: %ld us\n", ((end.tv_sec * 1000000 + end.tv_usec)
		                   - (start.tv_sec * 1000000 + start.tv_usec)));
  #ifndef SW_EMU
    sds_free(training_in0);
    sds_free(training_in1);
    sds_free(test_in);
    sds_free(result);
  #else 
    free(training_in0);
    free(training_in1);
    free(test_in);
    free(result);
  #endif

  return EXIT_SUCCESS;
}
