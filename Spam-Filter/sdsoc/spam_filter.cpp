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
#include <ctime>
#include <sys/time.h>

// other headers
#include "sgd.h"
#include "utils.h"
#include "check_result.h"

#ifndef SW_EMU
  #include "sds_lib.h"
#endif

int main(int argc, char *argv[]) 
{
  setbuf(stdout, NULL);

  printf("Spam Filter Application\n");

  // parse command line arguments
  std::string path_to_data("");
  parse_sdaccel_command_line_args(argc, argv, path_to_data);

  // allocate space
  // for software verification
  DataType*    data_points  = (DataType*)   malloc(sizeof(DataType)    * DATA_SET_SIZE);
  LabelType*   labels       = (LabelType*)  malloc(sizeof(LabelType)   * NUM_SAMPLES);
  FeatureType* param_vector = (FeatureType*)malloc(sizeof(FeatureType) * NUM_FEATURES); 
  // for compute
  #ifndef SW_EMU
    VectorDataType* data_points_for_accel = (VectorDataType*)   sds_alloc(sizeof(VectorDataType) * NUM_TRAINING * NUM_FEATURES / D_VECTOR_SIZE);
    VectorLabelType* labels_for_accel     = (VectorLabelType*)  sds_alloc(sizeof(VectorLabelType) * NUM_TRAINING / L_VECTOR_SIZE);
    VectorFeatureType* param_for_accel    = (VectorFeatureType*)sds_alloc(sizeof(VectorFeatureType) * NUM_FEATURES / F_VECTOR_SIZE);
  #else
    VectorDataType* data_points_for_accel = (VectorDataType*)   malloc(sizeof(VectorDataType) * NUM_TRAINING * NUM_FEATURES / D_VECTOR_SIZE);
    VectorLabelType* labels_for_accel     = (VectorLabelType*)  malloc(sizeof(VectorLabelType) * NUM_TRAINING / L_VECTOR_SIZE);
    VectorFeatureType* param_for_accel    = (VectorFeatureType*)malloc(sizeof(VectorFeatureType) * NUM_FEATURES / F_VECTOR_SIZE);
  #endif

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

  // run the accelerator
  struct timeval start, end;
  gettimeofday(&start, NULL);
  SgdLR(data_points_for_accel, labels_for_accel, param_for_accel);
  gettimeofday(&end, NULL);

  // reorganize the parameter vector
  for (int i = 0; i < NUM_FEATURES / F_VECTOR_SIZE; i ++ )
    for (int j = 0; j < F_VECTOR_SIZE; j ++ )
      param_vector[i*F_VECTOR_SIZE+j].range(FTYPE_TWIDTH-1, 0) = param_for_accel[i].range((j+1)*FTYPE_TWIDTH-1, j*FTYPE_TWIDTH);


  // check results

  // check results
  printf("Checking results:\n");
  check_results( param_vector, data_points, labels );
    
  printf("Elapsed time: %ld us\n", ((end.tv_sec * 1000000 + end.tv_usec)
		                   - (start.tv_sec * 1000000 + start.tv_usec)));

  // cleanup
  #ifndef SW_EMU
    sds_free(data_points_for_accel);
    sds_free(labels_for_accel);
    sds_free(param_for_accel);
  #else
    free(data_points_for_accel);
    free(labels_for_accel);
    free(param_for_accel);
  #endif
  free(data_points);
  free(labels);
  free(param_vector);

  return EXIT_SUCCESS;
}
