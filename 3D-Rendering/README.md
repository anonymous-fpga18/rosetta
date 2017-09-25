# 3D Rendering

## Description
This design renders 2D images from 3D models (3D triangle mesh). It is composed of the following four stages.
1. Projection: 3D triangle -> 2D triangle
2. Rasterization: search pixels in 2D triangle within the bounding box
3. Z-culling: hide or display pixels according to each pixel's "z" value (depth)
4. ColoringFB: coloring framebuffer according to the zbuffer

## Usage
### SDAccel
To execute or simulate the design, do

`./host.exe -f <path_to_xclbin_file>`

### SDSoC
To run software model, do

`./3d_rendering`

To run design on the board, do

`./rendering.elf`
