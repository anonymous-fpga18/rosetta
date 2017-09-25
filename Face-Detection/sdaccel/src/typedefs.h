/*===============================================================*/
/*                                                               */
/*                          typedefs.h                           */
/*                                                               */
/*                     Typedefs for the host                     */
/*                                                               */
/*===============================================================*/

#ifndef __HAAR_H__
#define __HAAR_H__

#define IMAGE_HEIGHT 240 
#define IMAGE_WIDTH 320
#define RESULT_SIZE 100

// target device
// change here to map to a different device
const std::string TARGET_DEVICE = "xilinx:adm-pcie-7v3:1ddr:3.0";

typedef struct MyPoint
{
  int x;
  int y;
} MyPoint;

typedef struct
{
  int width;
  int height;
} MySize;

typedef struct
{
  int x; 
  int y;
  int width;
  int height;
} MyRect;

typedef struct 
{
  int width;
  int height;
  int maxgrey;
  int flag;
} MyInputImage;

#endif
