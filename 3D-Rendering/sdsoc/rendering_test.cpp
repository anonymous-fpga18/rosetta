/*===============================================================*/
/*                                                               */
/*                        rendering.cpp                          */
/*                                                               */
/*                  Testbench for 3D Rendering                   */
/*                                                               */
/*===============================================================*/

// standard headers
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

// local header files
#include "rendering.h"
#include "timer.h"

// 3D triangles 
#include "input.dat"

// main function
int main( void )
{
  // input variables
  bit32 *input;
  input  = (bit32*)malloc(3 * NUM_3D_TRI * sizeof(bit32));
  // output values
  bit32 *output;
  output = (bit32*)malloc(NUM_FB * sizeof(bit32));

  // frame buffer
  bit8 frame_buffer_print[MAX_X][MAX_Y];

  // initialize timer 
  Timer timer("Bear FPGA");
  std::cout << "test started..." << "\n";
  
  // build input array
  for ( int i = 0; i < NUM_3D_TRI; i++)
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
  // invoke hardware function & start timer
  timer.start();
  rendering ( input, output );
  timer.stop();

  std::cout << "start read results..." << "\n";
  // read result
  int i = 0;
  int j = 0;
  bit32 temp;
  for ( int n = 0; n < NUM_FB; n++)
  {
     temp = output[n];
     frame_buffer_print[i][j++] = temp(7,0);
     frame_buffer_print[i][j++] = temp(15,8);
     frame_buffer_print[i][j++] = temp(23,16);
     frame_buffer_print[i][j++] = temp(31,24);
     if(j == MAX_Y)
     {
    	 i++;
    	 j = 0;
     }
  }

  std::cout << "start print results..." << "\n";
  // print result
  for ( int j = MAX_X-1; j >= 0; j--)
  {
    for ( int i = 0; i < MAX_Y; i++)
    {
      if (frame_buffer_print[i][j] < 10)
        std::cout << frame_buffer_print[i][j] << "  ";
      else if (frame_buffer_print[i][j] < 100)
        std::cout << frame_buffer_print[i][j] << " ";
      else if (frame_buffer_print[i][j] < 1000)
        std::cout << frame_buffer_print[i][j] << "";
    }
    std::cout << "\n";
  }

  free(input);
  free(output);
  return 0;   
}

